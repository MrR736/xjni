#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>

#include "xjni_hashmap.h"

#define TEST_ASSERT(cond, msg) \
	do { \
		if (!(cond)) { \
			fprintf(stderr, "❌ FAILED: %s\n", msg); \
			exit(EXIT_FAILURE); \
		} \
	} while (0)

#define THREAD_ITERATIONS 100000
#define THREAD_COUNT 4

/* ============================================================
 * Basic Functional Tests
 * ============================================================ */

void test_basic_insert_get() {
	printf("Running basic insert/get test...\n");

	XJNI_HashMap map;
	TEST_ASSERT(XJNI_HashMapInit(&map, 16), "Init failed");

	XJNI_HashMapInsert(&map, "apple", 10);

	XJNI_HashValue_t value = 0;
	TEST_ASSERT(XJNI_HashMapTryGet(&map, "apple", &value), "Key not found");
	TEST_ASSERT(value == 10, "Incorrect value");

	TEST_ASSERT(XJNI_HashMapCheckInvariant(&map), "Invariant failed");

	XJNI_HashMapFree(&map);
	printf("✔ basic insert/get passed\n");
}

void test_replace_and_compare() {
	printf("Running replace/compare test...\n");

	XJNI_HashMap map;
	XJNI_HashMapInit(&map, 16);

	XJNI_HashMapInsert(&map, "key", 5);

	TEST_ASSERT(XJNI_HashMapReplace(&map, "key", 20), "Replace failed");

	XJNI_HashValue_t old = 0;
	TEST_ASSERT(XJNI_HashMapReplaceGetOld(&map, "key", 30, &old), "ReplaceGetOld failed");
	TEST_ASSERT(old == 20, "Old value incorrect");

	TEST_ASSERT(XJNI_HashMapCompareReplace(&map, "key", 30, 50), "CompareReplace failed");
	TEST_ASSERT(!XJNI_HashMapCompareReplace(&map, "key", 30, 60), "CompareReplace should fail");

	XJNI_HashMapFree(&map);
	printf("✔ replace/compare passed\n");
}

void test_add_increment() {
	printf("Running add/increment test...\n");

	XJNI_HashMap map;
	XJNI_HashMapInit(&map, 16);

	XJNI_HashMapIncrement(&map, "counter");
	XJNI_HashMapIncrement(&map, "counter");
	XJNI_HashMapAdd(&map, "counter", 3);

	XJNI_HashValue_t value = 0;
	XJNI_HashMapTryGet(&map, "counter", &value);
	TEST_ASSERT(value == 5, "Add/Increment incorrect");

	XJNI_HashMapFree(&map);
	printf("✔ add/increment passed\n");
}

void test_delete_tombstone() {
	printf("Running delete/tombstone test...\n");

	XJNI_HashMap map;
	XJNI_HashMapInit(&map, 16);

	XJNI_HashMapInsert(&map, "a", 1);
	XJNI_HashMapInsert(&map, "b", 2);

	XJNI_HashMapDelete(&map, "a");

	XJNI_HashValue_t value = 0;
	TEST_ASSERT(!XJNI_HashMapTryGet(&map, "a", &value), "Deleted key still found");

	TEST_ASSERT(XJNI_HashMapCheckInvariant(&map), "Invariant after delete failed");

	XJNI_HashMapFree(&map);
	printf("✔ delete/tombstone passed\n");
}

void test_resize_pressure() {
	printf("Running resize pressure test...\n");

	XJNI_HashMap map;
	XJNI_HashMapInit(&map, 8);

	char key[64];

	for (XJNI_HashValue_t i = 0; i < 10000; i++) {
		snprintf(key, sizeof(key), "key_%"XJNI_HASH_FMT, i);
		XJNI_HashMapInsert(&map, key, i);
	}

	for (XJNI_HashValue_t i = 0; i < 10000; i++) {
		snprintf(key, sizeof(key), "key_%"XJNI_HASH_FMT, i);
		XJNI_HashValue_t value = 0;
		TEST_ASSERT(XJNI_HashMapTryGet(&map, key, &value), "Resize lookup failed");
		TEST_ASSERT(value == i, "Resize value incorrect");
	}

	TEST_ASSERT(XJNI_HashMapCheckInvariant(&map), "Invariant after resize failed");

	XJNI_HashMapFree(&map);
	printf("✔ resize pressure passed\n");
}

/* ============================================================
 * Multi-thread Test
 * ============================================================ */

typedef struct {
	XJNI_HashMap* map;
	int id;
} ThreadData;

void* thread_worker(void* arg) {
	ThreadData* data = (ThreadData*)arg;
	char key[64];

	for (XJNI_HashValue_t i = 0; i < THREAD_ITERATIONS; i++) {
		snprintf(key, sizeof(key), "thread_%d_key_%"XJNI_HASH_FMT, data->id, i);
		XJNI_HashMapInsert(data->map, key, i);
	}

	return NULL;
}
/* ============================================================
 * Multi-thread Test with per-thread timing
 * ============================================================ */

typedef struct {
	XJNI_HashMap* map;
	int id;
	double duration; // elapsed time in seconds
} ThreadDataTimed;

void* thread_worker_timed(void* arg) {
	ThreadDataTimed* data = (ThreadDataTimed*)arg;
	char key[64];

	clock_t start = clock();

	for (XJNI_HashValue_t i = 0; i < THREAD_ITERATIONS; i++) {
		snprintf(key, sizeof(key), "thread_%d_key_%"XJNI_HASH_FMT, data->id, i);
		XJNI_HashMapInsert(data->map, key, i);
	}

	clock_t end = clock();
	data->duration = (double)(end - start) / CLOCKS_PER_SEC;
	return NULL;
}

void test_multithread_timed() {
	printf("Running multithread test with per-thread timing...\n");

	XJNI_HashMap map;
	XJNI_HashMapInit(&map, 64);

	pthread_t threads[THREAD_COUNT];
	ThreadDataTimed threadData[THREAD_COUNT];

	// Start threads
	for (int i = 0; i < THREAD_COUNT; i++) {
		threadData[i].map = &map;
		threadData[i].id = i;
		threadData[i].duration = 0.0;
		pthread_create(&threads[i], NULL, thread_worker_timed, &threadData[i]);
	}

	// Join threads
	for (int i = 0; i < THREAD_COUNT; i++) {
		pthread_join(threads[i], NULL);
	}

	// Print per-thread performance
	for (int i = 0; i < THREAD_COUNT; i++) {
		double rate = THREAD_ITERATIONS / threadData[i].duration;
		printf("Thread %d: inserted %d keys in %.4f sec -> %.2f keys/sec\n",
			   i, THREAD_ITERATIONS, threadData[i].duration, rate);
	}

	TEST_ASSERT(XJNI_HashMapCheckInvariant(&map), "Invariant after multithread failed");

	XJNI_HashMapFree(&map);
	printf("✔ multithread timed passed\n");
}


void test_multithread() {
	printf("Running multithread test...\n");

	XJNI_HashMap map;
	XJNI_HashMapInit(&map, 64);

	pthread_t threads[THREAD_COUNT];
	ThreadData threadData[THREAD_COUNT];

	for (int i = 0; i < THREAD_COUNT; i++) {
		threadData[i].map = &map;
		threadData[i].id = i;
		pthread_create(&threads[i], NULL, thread_worker, &threadData[i]);
	}

	for (int i = 0; i < THREAD_COUNT; i++) {
		pthread_join(threads[i], NULL);
	}

	TEST_ASSERT(XJNI_HashMapCheckInvariant(&map), "Invariant after multithread failed");

	XJNI_HashMapFree(&map);
	printf("✔ multithread passed\n");
}

/* ============================================================
 * Timing helper
 * ============================================================ */
static void run_with_timer(void (*test_func)(), const char* test_name) {
	printf("=== Running %s ===\n", test_name);
	clock_t start = clock();
	test_func();
	clock_t end = clock();
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("⏱ %s took %.4f seconds\n\n", test_name, elapsed);
}

/* ============================================================
 * Main
 * ============================================================ */

int main() {
	printf("==== XJNI HashMap Full Test Suite ====\n\n");

	run_with_timer(test_basic_insert_get, "basic_insert_get");
	run_with_timer(test_replace_and_compare, "replace_and_compare");
	run_with_timer(test_add_increment, "add_increment");
	run_with_timer(test_delete_tombstone, "delete_tombstone");
	run_with_timer(test_resize_pressure, "resize_pressure");
	run_with_timer(test_multithread,"multithread");
	run_with_timer(test_multithread_timed,"multithread_timed");

	printf("\nALL TESTS PASSED\n");
	return 0;
}
