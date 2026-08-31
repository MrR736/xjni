/*
 * xjni_pool.c
 * Growable + Lock-Free Implementation
 */

#include "xjni_pool.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdatomic.h>

/* INTERNAL STRUCTURES */
typedef struct xjni_pool_chunk {
	void* memory;
	struct xjni_pool_chunk* next;
} xjni_pool_chunk;

typedef struct {
	xjni_pool base;				/* MUST remain first */
	xjni_pool_chunk* chunks;	/* slab list */
	size_t chunk_capacity;
	int growable;
	int lock_free;
	_Atomic(void*) atomic_free_list;
	atomic_size_t atomic_size;
} xjni_pool_internal;

#define INTERNAL(mp) ((xjni_pool_internal*)(mp))

/* ALIGNMENT */
static size_t align_size(size_t size) {
	size_t align = sizeof(max_align_t);
	size = (size + align - 1) & ~(align - 1);
	return size;
}

/* CHUNK CREATION */
static int add_chunk(xjni_pool_internal* ip) {
	size_t cap = ip->chunk_capacity;
	size_t total = cap * ip->base.element_size;

	void* memory = malloc(total);
	if (!memory)
		return 0;

	xjni_pool_chunk* chunk =
		(xjni_pool_chunk*)malloc(sizeof(*chunk));
	if (!chunk) {
		free(memory);
		return 0;
	}

	chunk->memory = memory;
	chunk->next = ip->chunks;
	ip->chunks = chunk;

	/* Build free list */
	for (size_t i = 0; i < cap; ++i) {
		void* elem = (uint8_t*)memory + i * ip->base.element_size;
		if (ip->lock_free) {
			void* head;
			do {
				head = atomic_load_explicit(
					&ip->atomic_free_list,
					memory_order_relaxed);

				*(void**)elem = head;

			} while (!atomic_compare_exchange_weak_explicit(
						&ip->atomic_free_list,
						&head,
						elem,
						memory_order_release,
						memory_order_relaxed));
		} else {
			*(void**)elem = ip->base.free_list;
			ip->base.free_list = elem;
		}
	}
	ip->base.capacity += cap;
	return 1;
}

/* CREATE */
xjni_pool* xjni_pool_Create(size_t capacity,size_t element_size,int use_mutex) {
	if (capacity == 0 || element_size == 0) return NULL;
	element_size = align_size(element_size);

	xjni_pool_internal* ip = (xjni_pool_internal*)calloc(1, sizeof(*ip));
	if (!ip) return NULL;

	ip->chunk_capacity = capacity;
	ip->growable = 1;
	ip->lock_free = use_mutex ? 0 : 1;

	ip->base.element_size = element_size;
	ip->base.capacity = 0;
	ip->base.size = 0;
	ip->base.free_list = NULL;
	ip->base.UseMutex = use_mutex ? 1 : 0;

	if (use_mutex) {
		if (xjni_mutex_init(&ip->base.mutex) != 0) {
			free(ip);
			return NULL;
		}
	}

	atomic_init(&ip->atomic_free_list, NULL);
	atomic_init(&ip->atomic_size, 0);

	if (!add_chunk(ip)) {
		if (use_mutex) xjni_mutex_destroy(&ip->base.mutex);
		free(ip);
		return NULL;
	}

	return &ip->base;
}

/* ALLOC */
void* xjni_pool_AllocFromPool(xjni_pool* mp) {
	if (!mp) return NULL;

	xjni_pool_internal* ip = INTERNAL(mp);

	/* LOCK-FREE */
	if (ip->lock_free) {
		void* head;
		void* next;

		for (;;) {
			head = atomic_load_explicit(
				&ip->atomic_free_list,
				memory_order_acquire);

			if (!head) {
				if (!ip->growable || !add_chunk(ip))
					return NULL;
				continue;
			}

			next = *(void**)head;

			if (atomic_compare_exchange_weak_explicit(
					&ip->atomic_free_list,
					&head,
					next,
					memory_order_release,
					memory_order_relaxed))
				break;
		}

		atomic_fetch_add_explicit(
			&ip->atomic_size,
			1,
			memory_order_relaxed);

		return head;
	}

	/* MUTEX */
	if (mp->UseMutex) xjni_mutex_lock(&mp->mutex);
	if (!mp->free_list) {
		if (!ip->growable || !add_chunk(ip)) {
			if (mp->UseMutex) xjni_mutex_unlock(&mp->mutex);
			return NULL;
		}
	}

	void* elem = mp->free_list;
	mp->free_list = *(void**)elem;
	mp->size++;

	if (mp->UseMutex)
		xjni_mutex_unlock(&mp->mutex);

	return elem;
}

/* FREE ELEMENT */

void xjni_pool_FreeElement(xjni_pool* mp, void* elem) {
	if (!mp || !elem) return;
	xjni_pool_internal* ip = INTERNAL(mp);

	/* LOCK-FREE */

	if (ip->lock_free) {
		void* head;

		do {
			head = atomic_load_explicit(
				&ip->atomic_free_list,
				memory_order_acquire);

			*(void**)elem = head;

		} while (!atomic_compare_exchange_weak_explicit(
					&ip->atomic_free_list,
					&head,
					elem,
					memory_order_release,
					memory_order_relaxed));

		atomic_fetch_sub_explicit(
			&ip->atomic_size,
			1,
			memory_order_relaxed);

		return;
	}

	/* MUTEX */
	if (mp->UseMutex) xjni_mutex_lock(&mp->mutex);

	*(void**)elem = mp->free_list;
	mp->free_list = elem;
	mp->size--;

	if (mp->UseMutex)
		xjni_mutex_unlock(&mp->mutex);
}

/* FREE POOL */

int xjni_pool_Free(xjni_pool* mp) {
	if (!mp)
		return XJNI_POOL_ERROR_NULL;

	xjni_pool_internal* ip = INTERNAL(mp);

	/* check in-use */

	if (ip->lock_free) {
		if (atomic_load_explicit(&ip->atomic_size,memory_order_acquire) != 0)
			return XJNI_POOL_ERROR_IN_USE;
	} else {
		if (mp->UseMutex) xjni_mutex_lock(&mp->mutex);

		if (mp->size != 0) {
			if (mp->UseMutex) xjni_mutex_unlock(&mp->mutex);
			return XJNI_POOL_ERROR_IN_USE;
		}

		if (mp->UseMutex) xjni_mutex_unlock(&mp->mutex);
	}

	/* free slabs */

	xjni_pool_chunk* c = ip->chunks;
	while (c) {
		xjni_pool_chunk* next = c->next;
		free(c->memory);
		free(c);
		c = next;
	}

	if (mp->UseMutex) xjni_mutex_destroy(&mp->mutex);
	free(ip);
	return XJNI_POOL_OK;
}

/* DEBUG HELPERS */

#ifdef DEBUG
size_t xjni_pool_UsedCount(xjni_pool* mp) {
	if (!mp) return 0;
	xjni_pool_internal* ip = INTERNAL(mp);
	if (ip->lock_free) return atomic_load_explicit(&ip->atomic_size,memory_order_relaxed);
	return mp->size;
}

size_t xjni_pool_FreeCount(xjni_pool* mp) {
	if (!mp) return 0;
	return mp->capacity - xjni_pool_UsedCount(mp);
}

size_t xjni_pool_Capacity(xjni_pool* mp) {
	return mp ? mp->capacity : 0;
}

#endif
