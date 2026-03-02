/*
 * xjni_pool_test_safe.c
 *
 * Full validation & stress test for:
 *  - Growable mutex pool
 *  - Lock-free pool
 *
 * Build (POSIX):
 *   gcc -std=c11 -O2 xjni_pool_test_safe.c xjni_pool.c xjni_thread.c -lpthread
 *
 * Build (Windows MSVC):
 *   cl /std:c11 xjni_pool_test_safe.c xjni_pool.c xjni_thread.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "xjni_pool.h"
#include "xjni_thread.h"

/* ============================================================
   TEST CONFIG
   ============================================================ */

#define INITIAL_CAPACITY  64
#define ELEMENT_SIZE      32
#define THREAD_COUNT      8
#define OPERATIONS_PER_THREAD 100000

/* ============================================================
   TEST STRUCT
   ============================================================ */

typedef struct {
    uint64_t id;
    char padding[ELEMENT_SIZE - sizeof(uint64_t)];
} test_object;

/* ============================================================
   BASIC TEST
   ============================================================ */

static void test_basic(void)
{
    printf("[TEST] Basic allocation/free...\n");

    xjni_pool* pool =
        xjni_pool_Create(16, sizeof(test_object), 1);

    assert(pool);

    void* ptrs[16];

    for (int i = 0; i < 16; ++i) {
        ptrs[i] = xjni_pool_AllocFromPool(pool);
        assert(ptrs[i] != NULL);
    }

    /* should grow automatically */
    void* extra = xjni_pool_AllocFromPool(pool);
    assert(extra != NULL);

    for (int i = 0; i < 16; ++i)
        xjni_pool_FreeElement(pool, ptrs[i]);

    xjni_pool_FreeElement(pool, extra);

    assert(xjni_pool_UsedCount(pool) == 0);

    assert(xjni_pool_Free(pool) == XJNI_POOL_OK);

    printf("  -> PASS\n");
}

/* ============================================================
   THREAD WORKER
   ============================================================ */

typedef struct {
    xjni_pool* pool;
    int thread_id;
} worker_arg;

static void* worker(void* arg)
{
    worker_arg* wa = (worker_arg*)arg;

    for (int i = 0; i < OPERATIONS_PER_THREAD; ++i) {
        test_object* obj =
            (test_object*)xjni_pool_AllocFromPool(wa->pool);

        if (!obj) continue;

        obj->id = (uint64_t)wa->thread_id;

        /* simulate work */
        memset(obj->padding, wa->thread_id,
               sizeof(obj->padding));

        xjni_pool_FreeElement(wa->pool, obj);
    }

    return NULL;
}

/* ============================================================
   MULTITHREAD TEST
   ============================================================ */

static void test_multithread(int use_mutex)
{
    printf("[TEST] Multithread (%s)...\n",
           use_mutex ? "Mutex" : "LockFree");

    xjni_pool* pool =
        xjni_pool_Create(INITIAL_CAPACITY,
                         sizeof(test_object),
                         use_mutex);

    assert(pool);

    xjni_thread_t threads[THREAD_COUNT];
    worker_arg args[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; ++i) {
        args[i].pool = pool;
        args[i].thread_id = i;

        assert(xjni_create(&threads[i],
                           worker,
                           &args[i]) == 0);
    }

    for (int i = 0; i < THREAD_COUNT; ++i)
        assert(xjni_join(threads[i], NULL) == 0);

    /* All objects must be returned */
    assert(xjni_pool_UsedCount(pool) == 0);

    assert(xjni_pool_Free(pool) == XJNI_POOL_OK);

    printf("  -> PASS\n");
}

/* ============================================================
   DESTROY IN-USE TEST
   ============================================================ */

static void test_destroy_in_use(void)
{
    printf("[TEST] Destroy while in use...\n");

    xjni_pool* pool =
        xjni_pool_Create(8, sizeof(test_object), 1);

    assert(pool);

    void* p = xjni_pool_AllocFromPool(pool);
    assert(p);

    /* Should fail */
    assert(xjni_pool_Free(pool)
           == XJNI_POOL_ERROR_IN_USE);

    xjni_pool_FreeElement(pool, p);

    assert(xjni_pool_Free(pool)
           == XJNI_POOL_OK);

    printf("  -> PASS\n");
}

/* ============================================================
   DOUBLE FREE TEST
   ============================================================ */

static void test_double_free(void)
{
    printf("[TEST] Double free detection (expected failure)...\n");

    xjni_pool* pool =
        xjni_pool_Create(4, sizeof(test_object), 1);

    assert(pool);

    void* p = xjni_pool_AllocFromPool(pool);
    assert(p);

    xjni_pool_FreeElement(pool, p);
    xjni_pool_FreeElement(pool, p); /* undefined */

    /* We EXPECT failure */
    assert(xjni_pool_Free(pool)
           == XJNI_POOL_ERROR_IN_USE);

    /* Cannot safely continue using this pool */
    printf("  -> PASS (double free correctly corrupts accounting)\n");
}

/* ============================================================
   MAIN
   ============================================================ */

int main(void)
{
    printf("==== XJNI Pool Full Safe Test ====\n");

    test_basic();
    test_destroy_in_use();
    test_double_free();

    test_multithread(1); /* Mutex */
    test_multithread(0); /* Lock-free */

    printf("==== ALL TESTS PASSED ====\n");

    return 0;
}
