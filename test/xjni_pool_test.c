/*
 * xjni_pool_test.c
 *
 * Full validation & stress test for:
 *  - Growable mutex pool
 *  - Lock-free pool
 *  - Double-free detection
 *  - Foreign-pointer rejection
 *  - Destroy-while-in-use
 *
 * POSIX build:
 *
 *   gcc -std=c11 -O2 -DDEBUG \
 *       xjni_pool_test_safe.c \
 *       xjni_pool.c \
 *       xjni_thread.c \
 *       -I. -pthread -latomic \
 *       -o xjni_pool_test_safe
 *
 * For x86-64 lock-free 128-bit CAS:
 *
 *   gcc -std=c11 -O2 -DDEBUG -mcx16 \
 *       xjni_pool_test_safe.c \
 *       xjni_pool.c \
 *       xjni_thread.c \
 *       -I. -pthread -latomic \
 *       -o xjni_pool_test_safe
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "xjni_pool.h"
#include "xjni_thread.h"

/* ============================================================
 *   TEST CONFIG
 *   ============================================================ */

#define INITIAL_CAPACITY          64
#define ELEMENT_SIZE              32
#define THREAD_COUNT              8
#define OPERATIONS_PER_THREAD     100000

/* ============================================================
 *   TEST STRUCT
 *   ============================================================ */

typedef struct {
	uint64_t id;
	char padding[ELEMENT_SIZE - sizeof(uint64_t)];
} test_object;

// 	BASIC TEST
static void test_basic(void) {
	printf("[TEST] Basic allocation/free...\n");
	xjni_pool* pool = xjni_pool_Create(16, sizeof(test_object), 1);
	assert(pool != NULL);
#ifdef DEBUG
	assert(xjni_pool_Capacity(pool) == 16);
	assert(xjni_pool_UsedCount(pool) == 0);
#endif
	void* ptrs[16];

	/*
	 * Consume the initial chunk completely.
	 */
	for (int i = 0; i < 16; ++i) {
		ptrs[i] = xjni_pool_AllocFromPool(pool);
		assert(ptrs[i] != NULL);
	}
#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 16);
#endif
	/*
	 * Pool must grow automatically.
	 */
	void* extra = xjni_pool_AllocFromPool(pool);
	assert(extra != NULL);
#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 17);
	assert(xjni_pool_Capacity(pool) >= 17);
#endif
	// Return all allocations.
	for (int i = 0; i < 16; ++i) xjni_pool_FreeElement(pool, ptrs[i]);
	xjni_pool_FreeElement(pool, extra);
#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 0);
	assert(xjni_pool_FreeCount(pool) ==
	xjni_pool_Capacity(pool));
#endif
	assert(xjni_pool_Free(pool) == XJNI_POOL_OK);
	printf("  -> PASS\n");
}

// THREAD WORKER
typedef struct {
	xjni_pool* pool;
	int thread_id;
} worker_arg;

static void* worker(void* arg) {
	worker_arg* wa = (worker_arg*)arg;
	for (int i = 0; i < OPERATIONS_PER_THREAD; ++i) {
		test_object* obj = (test_object*)xjni_pool_AllocFromPool(wa->pool);

		/*
		 * Allocation may legitimately fail only if the
		 * implementation has reached a hard resource limit.
		 *
		 * A growable pool normally should not fail here.
		 */
		if (!obj) continue;
		obj->id = (uint64_t)wa->thread_id;
		/*
		 * Simulate application use of the object.
		 */
		memset(obj->padding,wa->thread_id,sizeof(obj->padding));
		xjni_pool_FreeElement(wa->pool, obj);
	}
	return NULL;
}

// MULTITHREAD TEST
static void test_multithread(int use_mutex) {
	printf("[TEST] Multithread (%s)...\n", use_mutex ? "Mutex" : "LockFree");
	xjni_pool* pool = xjni_pool_Create(INITIAL_CAPACITY,sizeof(test_object),use_mutex);
	assert(pool != NULL);
	xjni_thread_t threads[THREAD_COUNT];
	worker_arg args[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; ++i) {
		args[i].pool = pool;
		args[i].thread_id = i;
		assert(xjni_create(&threads[i],worker,&args[i]) == 0);
	}
	for (int i = 0; i < THREAD_COUNT; ++i) assert(xjni_join(threads[i], NULL) == 0);
	/*
	 * Every worker frees every object it successfully
	 * allocated. Therefore no object may remain allocated.
	 */
#ifdef DEBUG
	size_t used = xjni_pool_UsedCount(pool);
	printf("  Used after workers: %zu\n", used);
	assert(used == 0);
	assert(xjni_pool_FreeCount(pool) == xjni_pool_Capacity(pool));
#endif
	// Destruction must succeed because nothing is in use.
	assert(xjni_pool_Free(pool) == XJNI_POOL_OK);
	printf("  -> PASS\n");
}

// DESTROY IN-USE TEST
static void test_destroy_in_use(void) {
	printf("[TEST] Destroy while in use...\n");
	xjni_pool* pool = xjni_pool_Create(8,sizeof(test_object),1);
	assert(pool != NULL);
	void* p = xjni_pool_AllocFromPool(pool);
	assert(p != NULL);
#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 1);
#endif
	// Pool cannot be destroyed while an allocation is outstanding.
	assert(xjni_pool_Free(pool) == XJNI_POOL_ERROR_IN_USE);
#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 1);
#endif
	// Return the outstanding object.
	xjni_pool_FreeElement(pool, p);
#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 0);
#endif
	// Now destruction must succeed.
	assert(xjni_pool_Free(pool) == XJNI_POOL_OK);
	printf("  -> PASS\n");
}

// DOUBLE FREE TEST
static void test_double_free(void) {
	printf("[TEST] Double free detection...\n");
	xjni_pool* pool = xjni_pool_Create(4,sizeof(test_object),1);
	assert(pool != NULL);
	void* p = xjni_pool_AllocFromPool(pool);
	assert(p != NULL);
#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 1);
#endif
	// First free is valid.
	xjni_pool_FreeElement(pool, p);
#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 0);
#endif

	/*
	 * Second free must be rejected.
	 *
	 * xjni_pool_FreeElement() has a void return type, so
	 * validation is performed through the accounting state.
	 *
	 * A correct implementation must NOT decrement the
	 * allocation count below zero or insert the same slot
	 * twice into the free list.
	 */
	xjni_pool_FreeElement(pool, p);

#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 0);
	assert(xjni_pool_FreeCount(pool) == xjni_pool_Capacity(pool));
#endif

	/*
	 * The pool must still be usable after the rejected
	 * double-free.
	 */
	void* p2 = xjni_pool_AllocFromPool(pool);
	assert(p2 != NULL);
#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 1);
#endif
	xjni_pool_FreeElement(pool, p2);
#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 0);
#endif
	/*
	 * A correctly detected double-free must NOT corrupt
	 * the pool, so destruction succeeds.
	 */
	assert(xjni_pool_Free(pool) == XJNI_POOL_OK);
	printf("  -> PASS\n");
}

// FOREIGN POINTER TEST
static void test_foreign_pointer(void) {
	printf("[TEST] Foreign pointer rejection...\n");
	xjni_pool* pool = xjni_pool_Create(8,sizeof(test_object),1);
	assert(pool != NULL);
	// This object does not belong to the pool.
	test_object foreign;
	memset(&foreign, 0, sizeof(foreign));
#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 0);
#endif
	// Must be ignored/rejected without corrupting the pool.
	xjni_pool_FreeElement(pool, &foreign);
#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 0);
#endif
	// Pool must remain fully usable.
	void* p = xjni_pool_AllocFromPool(pool);
	assert(p != NULL);
#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 1);
#endif
	xjni_pool_FreeElement(pool, p);
#ifdef DEBUG
	assert(xjni_pool_UsedCount(pool) == 0);
#endif
	assert(xjni_pool_Free(pool) == XJNI_POOL_OK);
	printf("  -> PASS\n");
}

/* ============================================================
 *   REPEATED ALLOCATION TEST
 *   ============================================================ */

static void test_repeated_allocation(void) {
	printf("[TEST] Repeated allocation/free...\n");
	xjni_pool* pool = xjni_pool_Create(8,sizeof(test_object),1);
	assert(pool != NULL);
	for (int i = 0; i < 10000; ++i) {
		test_object* obj = (test_object*)xjni_pool_AllocFromPool(pool);
		assert(obj != NULL);
		obj->id = (uint64_t)i;
		memset(obj->padding,(unsigned char)i,sizeof(obj->padding));
		xjni_pool_FreeElement(pool, obj);
#ifdef DEBUG
		assert(xjni_pool_UsedCount(pool) == 0);
#endif
	}
	assert(xjni_pool_Free(pool) == XJNI_POOL_OK);
	printf("  -> PASS\n");
}

// MAIN
int main(void) {
	printf("==== XJNI Pool Full Safe Test ====\n");
	test_basic();
	test_destroy_in_use();
	test_double_free();
	test_foreign_pointer();
	test_repeated_allocation();
	test_multithread(1);  /* Mutex */
	test_multithread(0);  /* Lock-free */
	printf("==== ALL TESTS PASSED ====\n");
	return 0;
}
