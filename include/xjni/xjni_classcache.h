/**
 * @file xjni_classcache.h
 * @brief Thread-safe LRU-based JNI class cache.
 *
 * Provides a fixed-size Least Recently Used (LRU) cache for JNI class
 * references. Cached classes are stored as global references to prevent
 * premature garbage collection.
 *
 * Designed for reducing repeated FindClass() calls and improving
 * performance in native-to-Java interactions.
 *
 * @author MrR736
 * @date 2026
 * @license GPL-3.0
 */

#ifndef XJNI_CLASSCACHE_H
#define XJNI_CLASSCACHE_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup XJNI_ClassCache ClassCache
 * @brief LRU-based JNI class caching utility.
 *
 * Maintains a fixed-capacity cache of jclass global references.
 * When capacity is exceeded, the least recently used class is evicted.
 *
 * Cached classes are stored as JNI global references and must be
 * released during library unload.
 *
 * @{
 */

/**
 * @brief Initialize the class cache.
 *
 * Must be called before any other cache operation.
 *
 * @return JNI_TRUE on success, JNI_FALSE on failure.
 *
 * @note Typically called from JNI_OnLoad.
 */
jboolean xjni_classcache_init(void);

/**
 * @brief Retrieve a cached class.
 *
 * If the class exists in cache:
 *   - Moves it to Most Recently Used (MRU) position
 *   - Returns its global jclass reference
 *
 * If the class does not exist:
 *   - Returns NULL
 *
 * @param env JNI environment pointer.
 * @param className Fully-qualified JNI class name
 *                  (e.g., "java/lang/String").
 *
 * @return Global jclass reference, or NULL if not found.
 *
 * @note Returned reference is owned by the cache.
 *       Do NOT call DeleteGlobalRef() on it.
 */
jclass xjni_classcache_get(JNIEnv* env,const char* className);

/**
 * @brief Insert a class into the cache.
 *
 * If the class already exists:
 *   - Moves it to MRU position
 *   - Returns existing global reference
 *
 * If cache is full:
 *   - Evicts the Least Recently Used (LRU) class
 *   - Deletes its global reference
 *
 * @param env JNI environment pointer.
 * @param className Fully-qualified JNI class name.
 * @param cls_local Local reference to class.
 *
 * @return Cached global jclass reference, or NULL on failure.
 *
 * @note The provided local reference may be converted into a
 *       global reference internally.
 * @warning Caller must not delete the returned global reference.
 */
jclass xjni_classcache_insert(JNIEnv* env,const char* className,jclass cls_local);

/**
 * @brief Clear the entire class cache.
 *
 * Deletes all stored global references and resets LRU state.
 *
 * @param env JNI environment pointer.
 *
 * @note Should be called during JNI_OnUnload.
 */
void xjni_classcache_clear(JNIEnv* env);

/** @} */ /* End of XJNI_ClassCache group */

#ifdef __cplusplus
}
#endif

#endif /* XJNI_CLASSCACHE_H */
