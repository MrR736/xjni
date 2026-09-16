/*
 * xjni_pool.c
 *
 * C11 memory pool
 *
 * Features:
 *   - Fixed-size elements
 *   - Growable chunks
 *   - Mutex implementation
 *   - Genuine lock-free allocation/free fast path on x86-64
 *   - ABA-resistant lock-free free list
 *   - Double-free detection
 *   - Foreign-pointer detection
 *   - ASan/TSan-friendly allocator metadata
 *
 * Lock-free head:
 *
 *       { generation, global slot index }
 *
 * represented as a 64-bit atomic value.
 *
 * Layout:
 *
 *       63                    32 31                     0
 *      +-----------------------+-------------------------+
 *      |    generation (32)    |    slot index (32)     |
 *      +-----------------------+-------------------------+
 *
 * This intentionally uses 64-bit atomics rather than 128-bit atomics.
 *
 * The target platform must provide genuine lock-free 64-bit atomics.
 *
 * The free-list metadata is stored separately from user memory.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <limits.h>

#define XJNI_BUILD
#include "xjni_pool.h"

/*
 * Configuration
 *
 * The lock-free implementation requires a native 64-bit atomic operation.
 * GCC/Clang on x86-64 provide this through the normal atomic operations.
 */

#if __XJNI_x86_64__ && (defined(__GNUC__) || defined(__clang__))
# define XJNI_POOL_HAS_LOCKFREE_HEAD 1
#else
# define XJNI_POOL_HAS_LOCKFREE_HEAD 0
#endif

// Constants
#define XJNI_POOL_INVALID_INDEX UINT32_MAX

// Forward declarations
typedef struct xjni_pool_chunk xjni_pool_chunk;


/*
 * Slot metadata
 *
 * next:
 *     Global slot index of the next free element.
 *
 * state:
 *     0 = free
 *     1 = allocated
 *
 * The metadata is separate from user memory.
 */
typedef struct xjni_pool_slot {
	_Atomic(uint32_t) next;
	_Atomic(uint8_t) state;
} xjni_pool_slot;


// Chunk
struct xjni_pool_chunk {
	// User-visible element storage.
	void* memory;

	// Allocator metadata.
	xjni_pool_slot* slots;

	// Number of elements in this chunk.
	size_t capacity;

	/*
	 * Globally unique index of the first slot.
	 *
	 * Example:
	 *   chunk 0 -> base_index = 0
	 *   chunk 1 -> base_index = 64
	 *   chunk 2 -> base_index = 128
	 */
	uint32_t base_index;

	/*
	 * Atomically published chunk list.
	 *
	 * Chunks are never removed until destruction.
	 */
	_Atomic(xjni_pool_chunk*) next;
};


// Lock-free tagged head
#if XJNI_POOL_HAS_LOCKFREE_HEAD
typedef uint64_t xjni_pool_head_t;


/*
 * Layout:
 *     [ generation:32 ][ slot index:32 ]
 */
static xjni_pool_head_t make_head(uint32_t index,uint32_t generation) {
	return ((uint64_t)generation << 32) | (uint64_t)index;
}

static uint32_t head_index(xjni_pool_head_t head) {
	return (uint32_t)(head & UINT32_MAX);
}

static uint32_t head_generation(xjni_pool_head_t head) {
	return (uint32_t)(head >> 32);
}
#endif

/*
 * Internal pool
 *
 * base MUST remain the first member.
 */
typedef struct {
	xjni_pool base;
	// Atomically published chunk list.
	_Atomic(xjni_pool_chunk*) chunks;

	// Number of elements allocated per growth operation.
	size_t chunk_capacity;

	// Growable pool.
	int growable;

	/*
	 * 0 = mutex
	 * 1 = lock-free
	 */
	int lock_free;

#if XJNI_POOL_HAS_LOCKFREE_HEAD
	// ABA-resistant lock-free free-list head.
	_Atomic(xjni_pool_head_t) atomic_head;
#endif

	// Number of currently allocated elements in lock-free mode.
	atomic_size_t atomic_size;

	// Serializes chunk creation.
	xjni_mutex_t growth_mutex;

	//Lifecycle flag.
	atomic_bool destroying;
} xjni_pool_internal;

#define INTERNAL(mp) ((xjni_pool_internal*)(mp))

#define XJNI_LOCK(mp) if ((mp)->UseMutex) xjni_mutex_lock(&(mp)->mutex)
#define XJNI_UNLOCK(mp) if ((mp)->UseMutex) xjni_mutex_unlock(&(mp)->mutex)
#define XJNI_DESTROY(mp) if ((mp)->UseMutex) xjni_mutex_destroy(&(mp)->mutex)

// Utility
static int checked_mul_size(size_t a,size_t b,size_t* result) {
	if (!result) return 0;
	if (a != 0 && b > SIZE_MAX / a) return 0;
	*result = a * b;
	return 1;
}

static int checked_add_size(size_t a,size_t b,size_t* result) {
	if (!result) return 0;
	if (b > SIZE_MAX - a) return 0;
	*result = a + b;
	return 1;
}

static size_t align_size(size_t size) {
	const size_t align = sizeof(max_align_t);
	if (size > SIZE_MAX - (align - 1)) return 0;
	return (size + align - 1) & ~(align - 1);
}

// Atomic chunk-list access
static xjni_pool_chunk* load_chunks(xjni_pool_internal* ip) {
	return atomic_load_explicit(&ip->chunks,memory_order_acquire);
}

static void publish_chunk(xjni_pool_internal* ip,xjni_pool_chunk* chunk) {
	xjni_pool_chunk* old_head = atomic_load_explicit(&ip->chunks,memory_order_relaxed);
	atomic_store_explicit(&chunk->next,old_head,memory_order_relaxed);
	atomic_store_explicit(&ip->chunks,chunk,memory_order_release);
}

/*
 * Pointer lookup
 *
 * Finds the chunk and local slot index for a user pointer.
 *
 * The chunk list is immutable after publication:
 *     chunk -> next
 *
 * is only written before publication.
 * Therefore readers can safely traverse it using acquire loads.
 */
static xjni_pool_chunk* find_chunk_for_pointer(xjni_pool_internal* ip,const void* ptr,uint32_t* index_out) {
	if (!ip || !ptr) return NULL;
	const uintptr_t p = (uintptr_t)ptr;
	xjni_pool_chunk* chunk = load_chunks(ip);
	while (chunk) {
		const uintptr_t begin = (uintptr_t)chunk->memory;
		size_t bytes;
		if (!checked_mul_size(chunk->capacity,ip->base.element_size,&bytes)) {
			chunk = atomic_load_explicit(&chunk->next,memory_order_acquire);
			continue;
		}
		if (bytes > UINTPTR_MAX - begin) {
			chunk = atomic_load_explicit(&chunk->next,memory_order_acquire);
			continue;
		}
		const uintptr_t end =
		begin + bytes;

		if (p >= begin && p < end) {
			const uintptr_t offset = p - begin;
			if (ip->base.element_size == 0) return NULL;
			// Pointer must be exactly at an element boundary.
			if (offset % ip->base.element_size != 0) return NULL;
			const size_t index = offset / ip->base.element_size;
			if (index >= chunk->capacity) return NULL;
			if (index > UINT32_MAX) return NULL;
			if (index_out) *index_out = (uint32_t)index;
			return chunk;
		}
		chunk = atomic_load_explicit(&chunk->next,memory_order_acquire);
	}
	return NULL;
}

/*
 * Global slot index lookup
 *
 * Converts:
 *     global slot ID
 *
 * into:
 *     chunk + local slot index
 *
 * The chunk list is immutable after publication.
 */
static xjni_pool_chunk* find_chunk_for_index(xjni_pool_internal* ip,uint32_t global_index,uint32_t* local_index_out) {
	if (!ip) return NULL;
	xjni_pool_chunk* chunk = load_chunks(ip);
	while (chunk) {
		const uint64_t begin = (uint64_t)chunk->base_index;
		const uint64_t end = begin + (uint64_t)chunk->capacity;
		if ((uint64_t)global_index >= begin && (uint64_t)global_index < end) {
			const uint32_t local_index = global_index - chunk->base_index;
			if ((size_t)local_index >= chunk->capacity) return NULL;
			if (local_index_out) *local_index_out = local_index;
			return chunk;
		}
		chunk = atomic_load_explicit(&chunk->next,memory_order_acquire);
	}
	return NULL;
}

// Convert global slot index to element pointer
static void* element_from_index(xjni_pool_internal* ip,uint32_t global_index) {
	uint32_t local_index;
	xjni_pool_chunk* chunk = find_chunk_for_index(ip,global_index,&local_index);
	if (!chunk)return NULL;
	return (uint8_t*)chunk->memory + (size_t)local_index * ip->base.element_size;
}

// Lock-free head operations
#if XJNI_POOL_HAS_LOCKFREE_HEAD
static xjni_pool_head_t atomic_head_load(xjni_pool_internal* ip) {
	return atomic_load_explicit(&ip->atomic_head,memory_order_acquire);
}

static int atomic_head_cas(xjni_pool_internal* ip,xjni_pool_head_t* expected,xjni_pool_head_t desired) {
	return atomic_compare_exchange_weak_explicit(&ip->atomic_head,expected,desired,memory_order_acq_rel,memory_order_acquire);
}
#endif

// Initialize chunk metadata
static void initialize_chunk_slots(xjni_pool_chunk* chunk) {
	for (size_t i = 0; i < chunk->capacity; ++i) {
		atomic_init(&chunk->slots[i].next,XJNI_POOL_INVALID_INDEX);
		atomic_init(&chunk->slots[i].state,0);
	}
}

// Push entire chunk onto lock-free free list
#if XJNI_POOL_HAS_LOCKFREE_HEAD
static void push_chunk_lockfree(xjni_pool_internal* ip,xjni_pool_chunk* chunk) {
	for (size_t i = 0; i < chunk->capacity; ++i) {
		xjni_pool_slot* slot = &chunk->slots[i];
		const uint32_t index = chunk->base_index + (uint32_t)i;
		atomic_store_explicit(&slot->state,0,memory_order_relaxed);
		for (;;) {
			xjni_pool_head_t old_head = atomic_head_load(ip);
			const uint32_t old_index = head_index(old_head);
			const uint32_t generation = head_generation(old_head);
			// Link this slot to the current head.
			atomic_store_explicit(&slot->next,old_index,memory_order_relaxed);
			const xjni_pool_head_t new_head = make_head(index,generation + 1);
			if (atomic_head_cas(ip,&old_head,new_head)) break;
		}
	}
}
#endif

/*
 * Add chunk
 *
 * Caller must hold growth_mutex.
 */
static int add_chunk(xjni_pool_internal* ip) {
	if (!ip || ip->chunk_capacity == 0 || ip->base.element_size == 0)
		return 0;

	/*
	 * The lock-free head has a 32-bit global slot index.
	 */
	if (ip->lock_free) {
		if ((uint64_t)ip->base.capacity + (uint64_t)ip->chunk_capacity > UINT32_MAX)
			return 0;
	}

	size_t memory_size;

	if (!checked_mul_size(ip->chunk_capacity,ip->base.element_size,&memory_size)) {
		return 0;
	}

	xjni_pool_chunk* chunk = (xjni_pool_chunk*)calloc(1,sizeof(*chunk));
	if (!chunk) return 0;

	chunk->memory = malloc(memory_size);
	if (!chunk->memory) {
		free(chunk);
		return 0;
	}

	chunk->slots = (xjni_pool_slot*)calloc(ip->chunk_capacity,sizeof(*chunk->slots));
	if (!chunk->slots) {
		free(chunk->memory);
		free(chunk);
		return 0;
	}
	chunk->capacity = ip->chunk_capacity;

	/*
	 * base.capacity is the number of slots already published.
	 * Therefore it is the first global index of this chunk.
	 */
	if (ip->base.capacity > UINT32_MAX) {
		free(chunk->slots);
		free(chunk->memory);
		free(chunk);
		return 0;
	}
	chunk->base_index = (uint32_t)ip->base.capacity;
	atomic_init(&chunk->next,NULL);
	initialize_chunk_slots(chunk);
#if XJNI_POOL_HAS_LOCKFREE_HEAD
	if (ip->lock_free) {
		/*
		 * Publish the chunk before inserting its slots into
		 * the lock-free free list.
		 */
		publish_chunk(ip,chunk);
		push_chunk_lockfree(ip,chunk);
	} else
#endif
	{
		/*
		 * Mutex mode.
		 *
		 * Caller holds both:
		 *
		 *     growth_mutex
		 *     base.mutex
		 */
		publish_chunk(ip,chunk);
		for (size_t i = 0; i < chunk->capacity; ++i) {
			void* elem = (uint8_t*)chunk->memory + i * ip->base.element_size;
			*(void**)elem = ip->base.free_list;
			ip->base.free_list = elem;
		}
	}

	size_t new_capacity;

	if (!checked_add_size(ip->base.capacity,chunk->capacity,&new_capacity)) {
		/*
		 * At this point the chunk is already published.
		 *
		 * Reaching this condition means the pool cannot safely
		 * continue, because capacity accounting would overflow.
		 */
		abort();
	}

	ip->base.capacity = new_capacity;
	if (!ip->base.pool) ip->base.pool = chunk->memory;

	return 1;
}

// Create
XJNI_API xjni_pool* xjni_pool_Create(size_t capacity,size_t element_size,int use_mutex) {
	if (capacity == 0 || element_size == 0) {
		return NULL;
	}
	element_size = align_size(element_size);
	if (element_size == 0) return NULL;
	xjni_pool_internal* ip = (xjni_pool_internal*)calloc(1,sizeof(*ip));
	if (!ip) return NULL;
	ip->chunk_capacity = capacity;
	ip->growable = 1;

	/*
	 * API:
	 *     use_mutex != 0 -> mutex
	 *     use_mutex == 0 -> lock-free
	 */
	ip->lock_free = use_mutex ? 0 : 1;
	ip->base.pool = NULL;
	ip->base.free_list = NULL;
	ip->base.capacity = 0;
	ip->base.size = 0;
	ip->base.element_size = element_size;
	ip->base.UseMutex = use_mutex ? 1 : 0;
	atomic_init(&ip->chunks,NULL);
	atomic_init(&ip->atomic_size,0);
	atomic_init(&ip->destroying,false);

#if XJNI_POOL_HAS_LOCKFREE_HEAD
	atomic_init(&ip->atomic_head,make_head(XJNI_POOL_INVALID_INDEX,0));
	if (ip->lock_free) {
		/*
		 * Never claim lock-free operation unless the platform
		 * genuinely provides lock-free 64-bit atomics.
		 */
		if (!atomic_is_lock_free(&ip->atomic_head)) {
			free(ip);
			return NULL;
		}
	}
#else
	if (ip->lock_free) {
		// This build cannot provide the promised lock-free path.
		free(ip);
		return NULL;
	}
#endif

	if (xjni_mutex_init(&ip->growth_mutex) != 0) {
		free(ip);
		return NULL;
	}

	if (use_mutex) {
		if (xjni_mutex_init(&ip->base.mutex) != 0) {
			xjni_mutex_destroy(&ip->growth_mutex);
			free(ip);
			return NULL;
		}
	}

	// Mutex mode requires base.mutex while manipulating free_list.
	if (use_mutex) xjni_mutex_lock(&ip->base.mutex);

	// Initial chunk.
	xjni_mutex_lock(&ip->growth_mutex);

	const int ok = add_chunk(ip);

	if (use_mutex) xjni_mutex_unlock(&ip->base.mutex);

	xjni_mutex_unlock(&ip->growth_mutex);

	if (!ok) {
		if (use_mutex) xjni_mutex_destroy(&ip->base.mutex);
		xjni_mutex_destroy(&ip->growth_mutex);
		free(ip);
		return NULL;
	}

	return &ip->base;
}

// Lock-free allocation
#if XJNI_POOL_HAS_LOCKFREE_HEAD
static void* alloc_lockfree(xjni_pool_internal* ip) {
	for (;;) {
		if (atomic_load_explicit(&ip->destroying,memory_order_acquire)) {
			return NULL;
		}
		xjni_pool_head_t old_head = atomic_head_load(ip);
		const uint32_t index = head_index(old_head);

		// Empty free list.
		if (index == XJNI_POOL_INVALID_INDEX) {
			if (!ip->growable) return NULL;
			xjni_mutex_lock(&ip->growth_mutex);
			// Recheck after acquiring growth mutex.
			xjni_pool_head_t current = atomic_head_load(ip);
			if (head_index(current) == XJNI_POOL_INVALID_INDEX) {
				const int ok = add_chunk(ip);
				xjni_mutex_unlock(&ip->growth_mutex);
				if (!ok)return NULL;
			} else xjni_mutex_unlock(&ip->growth_mutex);
			continue;
		}

		// Resolve the global slot index.
		uint32_t local_index;

		xjni_pool_chunk* chunk = find_chunk_for_index(ip,index,&local_index);
		if (!chunk) abort();

		xjni_pool_slot* slot = &chunk->slots[local_index];

		// Read the next index before removing the head.
		const uint32_t next = atomic_load_explicit(&slot->next,memory_order_acquire);
		const xjni_pool_head_t new_head = make_head(next,head_generation(old_head) + 1);

		// Remove the node.
		if (!atomic_head_cas(ip,&old_head,new_head)) { continue; }

		/*
		 * Ownership transition:
		 *     FREE -> ALLOCATED
		 */
		uint8_t expected = 0;

		if (!atomic_compare_exchange_strong_explicit(
			&slot->state,
			&expected,1,
			memory_order_acquire,
			memory_order_relaxed)) {
			/*
			 * A node in the free list must be free.
			 *
			 * If this happens, allocator metadata is corrupt.
			 */
			abort();
		}
		atomic_fetch_add_explicit(&ip->atomic_size,1,memory_order_relaxed);
		return (uint8_t*)chunk->memory + (size_t)local_index * ip->base.element_size;
	}
}
#endif

// Allocation
XJNI_API void* xjni_pool_AllocFromPool(xjni_pool* mp) {
	if (!mp) return NULL;
	xjni_pool_internal* ip = INTERNAL(mp);
	if (atomic_load_explicit(&ip->destroying,memory_order_acquire)) { return NULL; }
#if XJNI_POOL_HAS_LOCKFREE_HEAD
	if (ip->lock_free) return alloc_lockfree(ip);
#endif

	// Mutex implementation
	XJNI_LOCK(mp);
	if (!mp->free_list) {
		// Recheck while holding both locks.
		xjni_mutex_lock(&ip->growth_mutex);
		if (!mp->free_list) {
			if (!ip->growable || !add_chunk(ip)) {
				xjni_mutex_unlock(&ip->growth_mutex);
				XJNI_UNLOCK(mp);
				return NULL;
			}
		}
		xjni_mutex_unlock(&ip->growth_mutex);
	}

	void* elem = mp->free_list;
	if (!elem) {
		XJNI_UNLOCK(mp);
		return NULL;
	}

	// Remove from free list.
	mp->free_list = *(void**)elem;

	// Locate allocator metadata.
	uint32_t index;
	xjni_pool_chunk* chunk = find_chunk_for_pointer(ip,elem,&index);
	if (!chunk) {
		XJNI_UNLOCK(mp);
		abort();
	}
	xjni_pool_slot* slot = &chunk->slots[index];

	/*
	 * Ownership transition:
	 *     FREE -> ALLOCATED
	 */
	uint8_t expected = 0;

	if (!atomic_compare_exchange_strong_explicit(
		&slot->state,&expected,1,
		memory_order_acquire,memory_order_relaxed)) {
		XJNI_UNLOCK(mp);
		abort();
	}
	mp->size++;
	XJNI_UNLOCK(mp);
	return elem;
}

// Lock-free free
#if XJNI_POOL_HAS_LOCKFREE_HEAD
static int free_lockfree(xjni_pool_internal* ip,xjni_pool_chunk* chunk,uint32_t local_index,uint32_t global_index) {
	xjni_pool_slot* slot = &chunk->slots[local_index];

	/*
	 * Ownership transition:
	 *     ALLOCATED -> FREE
	 *
	 * Failure means that the element is already free.
	 */
	uint8_t expected = 1;

	if (!atomic_compare_exchange_strong_explicit(
		&slot->state,&expected,0,
		memory_order_release,memory_order_relaxed)) {

		return 0;
	}

	// Publish the node into the lock-free free list.
	for (;;) {
		xjni_pool_head_t old_head = atomic_head_load(ip);
		const uint32_t old_index = head_index(old_head);
		const uint32_t generation = head_generation(old_head);
		// Link this node to the current head.
		atomic_store_explicit(&slot->next,old_index,memory_order_relaxed);
		const xjni_pool_head_t new_head = make_head(global_index,generation + 1);
		if (atomic_head_cas(ip,&old_head,new_head)) {
			atomic_fetch_sub_explicit(&ip->atomic_size,1,memory_order_relaxed);
			return 1;
		}
	}
}
#endif


// Free element
XJNI_API void xjni_pool_FreeElement(xjni_pool* mp,void* elem) {
	if (!mp || !elem) return;
	xjni_pool_internal* ip = INTERNAL(mp);
	if (atomic_load_explicit(&ip->destroying,memory_order_acquire)) return;

	// Mutex mode
	if (!ip->lock_free) {
		XJNI_LOCK(mp);
		uint32_t index;
		xjni_pool_chunk* chunk = find_chunk_for_pointer(ip,elem,&index);

		// Foreign pointer or misaligned pointer.
		if (!chunk) {
			XJNI_UNLOCK(mp);
			return;
		}
		xjni_pool_slot* slot = &chunk->slots[index];

		// Detect double free.
		uint8_t expected = 1;

		if (!atomic_compare_exchange_strong_explicit(
			&slot->state,&expected,0,
			memory_order_release,memory_order_relaxed)) {
			XJNI_UNLOCK(mp);
			return;
		}

		/*
		 * Return to mutex free list.
		 *
		 * User memory is available for allocator metadata because
		 * the object is now free and the mutex serializes access.
		 */
		*(void**)elem = mp->free_list;
		mp->free_list = elem;
		if (mp->size == 0) {
			XJNI_UNLOCK(mp);
			abort();
		}
		mp->size--;
		XJNI_UNLOCK(mp);
		return;
	}

	// Lock-free mode
#if XJNI_POOL_HAS_LOCKFREE_HEAD
	uint32_t local_index;
	xjni_pool_chunk* chunk = find_chunk_for_pointer(ip,elem,&local_index);
	if (!chunk) return;
	// Verify exact element address.
	void* expected_elem = (uint8_t*)chunk->memory + (size_t)local_index * ip->base.element_size;
	if (expected_elem != elem) return;
	const uint64_t global_index64 = (uint64_t)chunk->base_index + (uint64_t)local_index;
	if (global_index64 > UINT32_MAX) return;
	(void)free_lockfree(ip,chunk,local_index,(uint32_t)global_index64);
#else
	(void)ip;
	(void)elem;
#endif
}

/*
 * Free pool
 *
 * IMPORTANT:
 *
 * xjni_pool_Free() is a lifecycle operation.
 *
 * The caller must guarantee that no thread can concurrently enter
 * AllocFromPool() or FreeElement() after destruction begins.
 *
 * The destroying flag prevents new operations from proceeding, but
 * cannot retroactively protect an operation that has already entered
 * the API.
 */
XJNI_API int xjni_pool_Free(xjni_pool* mp) {
	if (!mp) return XJNI_POOL_ERROR_NULL;
	xjni_pool_internal* ip = INTERNAL(mp);
	bool expected = false;
	if (!atomic_compare_exchange_strong_explicit(
		&ip->destroying,
		&expected,
		true,
		memory_order_acq_rel,memory_order_acquire)) {
		return XJNI_POOL_ERROR_IN_USE;
	}

	// Lock-free mode
	if (ip->lock_free) {
		const size_t used = atomic_load_explicit(&ip->atomic_size,memory_order_acquire);
		if (used != 0) {
			atomic_store_explicit(&ip->destroying,false,memory_order_release);
			return XJNI_POOL_ERROR_IN_USE;
		}
	} else {
		// Mutex mode
		XJNI_LOCK(mp);
		const size_t used =mp->size;
		if (used != 0) {
			if (mp->UseMutex)xjni_mutex_unlock(&mp->mutex);
			atomic_store_explicit(&ip->destroying,false,memory_order_release);
			return XJNI_POOL_ERROR_IN_USE;
		}
		XJNI_UNLOCK(mp);
	}

	/*
	 * No elements are currently allocated.
	 *
	 * Caller must ensure no concurrent operations are active.
	 */
	xjni_pool_chunk* chunk = atomic_load_explicit(&ip->chunks,memory_order_acquire);

	while (chunk) {
		xjni_pool_chunk* next = atomic_load_explicit( &chunk->next,memory_order_acquire);
		free(chunk->slots);
		free(chunk->memory);
		free(chunk);
		chunk = next;
	}

	XJNI_DESTROY(mp);
	xjni_mutex_destroy(&ip->growth_mutex);
	free(ip);
	return XJNI_POOL_OK;
}

// Debug helpers
#ifdef DEBUG
XJNI_API size_t xjni_pool_UsedCount(xjni_pool* mp) {
	if (!mp) return 0;
	xjni_pool_internal* ip = INTERNAL(mp);
	if (ip->lock_free) { return atomic_load_explicit(&ip->atomic_size,memory_order_acquire); }
	return mp->size;
}

XJNI_API size_t xjni_pool_FreeCount(xjni_pool* mp) {
	if (!mp) return 0;
	const size_t capacity = mp->capacity;
	const size_t used = xjni_pool_UsedCount(mp);
	return used <= capacity ? capacity - used : 0;
}

XJNI_API size_t xjni_pool_Capacity(xjni_pool* mp) {
	return mp ? mp->capacity : 0;
}
#endif
