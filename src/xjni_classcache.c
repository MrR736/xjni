#include <stdlib.h>
#include <string.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni_thread.h>
#include <xjni_string.h>
#include <xjni_classcache.h>
#include <xjni_hashmap.h>

/**
 * @def XJNI_CLASSCACHE_MAX
 * @brief Maximum number of classes stored in cache.
 */
#define XJNI_CLASSCACHE_MAX 64

/**
 * @struct XJNI_ClassCache
 * @brief Internal node used by the LRU linked list.
 *
 * Each node represents a cached class entry.
 *
 * @note This structure is managed internally and should not be
 *       modified directly by users.
 */
typedef struct XJNI_ClassCache {
    char* className;                  /**< Fully-qualified JNI class name */
    jclass cls;                       /**< Global reference to class */
    struct XJNI_ClassCache* prev;     /**< Previous node in LRU list */
    struct XJNI_ClassCache* next;     /**< Next node in LRU list */
} XJNI_ClassCache;

static XJNI_ClassCache g_nodes[XJNI_CLASSCACHE_MAX];
static XJNI_HashMap g_map;

static XJNI_ClassCache* g_lruHead = NULL; // MRU
static XJNI_ClassCache* g_lruTail = NULL; // LRU

static xjni_mutex_t g_mutex;
static void xjni_classcache_mutex_init() {
	static int inited = 0;
	if (!inited) {
		xjni_mutex_init(&g_mutex);
		inited = 1;
	}
}
#define LOCK() xjni_mutex_lock(&g_mutex)
#define UNLOCK() xjni_mutex_unlock(&g_mutex)

static void moveToHead(XJNI_ClassCache* node) {
	if (!node || node == g_lruHead) return;
	if (node->prev) node->prev->next = node->next;
	if (node->next) node->next->prev = node->prev;
	if (node == g_lruTail) g_lruTail = node->prev;
	node->prev = NULL;
	node->next = g_lruHead;
	if (g_lruHead) g_lruHead->prev = node;
	g_lruHead = node;
	if (!g_lruTail) g_lruTail = node;
}

jboolean xjni_classcache_init(void) {
	xjni_classcache_mutex_init();
	LOCK();
	jmemset(g_nodes, 0, sizeof(g_nodes));
	XJNI_HashMapInit(&g_map, XJNI_CLASSCACHE_MAX * 2); // load factor ~0.5
	g_lruHead = g_lruTail = NULL;
	UNLOCK();
	return JNI_TRUE;
}

jclass xjni_classcache_get(JNIEnv* env, const char* className) {
	if (!className || !env) return NULL;
	LOCK();
	XJNI_HashValue_t idx;
	if (!XJNI_HashMapTryGet(&g_map, className, &idx)) {
		UNLOCK();
		return NULL;
	}
	XJNI_ClassCache* node = &g_nodes[idx];
	moveToHead(node);
	UNLOCK();
	return node->cls;
}

jclass xjni_classcache_insert(JNIEnv* env, const char* className, jclass cls_local) {
	if (!className || !cls_local || !env) return NULL;
	LOCK();

	XJNI_HashValue_t idx;
	if (XJNI_HashMapTryGet(&g_map, className, &idx)) {
		// already cached, update MRU
		XJNI_ClassCache* node = &g_nodes[idx];
		moveToHead(node);
		UNLOCK();
		return node->cls;
	}

	// Find empty slot or evict LRU
	XJNI_ClassCache* node = NULL;
	for (int i = 0; i < XJNI_CLASSCACHE_MAX; i++) {
		if (!g_nodes[i].cls) {
			node = &g_nodes[i];
			idx = i;
			break;
		}
	}
	if (!node) {
		// Evict LRU
		node = g_lruTail;
		idx = (int)(node - g_nodes);
		// Remove from hash map
		XJNI_HashMapDelete(&g_map, node->className);
		// Free previous data
		free(node->className);
		node->className = NULL;
		if (node->cls) {
			_DeleteGlobalRef(env, node->cls);
			node->cls = NULL;
		}
	}

	// Insert new class
	node->cls = _NewGlobalRef(env, cls_local);
	node->className = strdup(className);
	node->prev = node->next = NULL;
	moveToHead(node);
	XJNI_HashMapInsert(&g_map, className, idx);

	UNLOCK();
	return node->cls;
}

void xjni_classcache_clear(JNIEnv* env) {
	if (!env) return;
	LOCK();
	for (int i = 0; i < XJNI_CLASSCACHE_MAX; i++) {
		if (g_nodes[i].cls) _DeleteGlobalRef(env, g_nodes[i].cls);
		g_nodes[i].cls = NULL;
		if (g_nodes[i].className) free(g_nodes[i].className);
		g_nodes[i].className = NULL;
		g_nodes[i].prev = g_nodes[i].next = NULL;
	}
	XJNI_HashMapClear(&g_map);
	g_lruHead = g_lruTail = NULL;
	UNLOCK();
}
