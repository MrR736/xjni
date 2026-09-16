/**
 * @file xjni_class.h
 * @brief XJNI Java Object <-> C++ Class Mapping Utilities
 * @date 2026
 * @author MrR736
 * @license MIT
 *
 * Generic callback-based Java object <-> native C++ object bridge.
 */

#ifndef __XJNI_CLASS_H__
#define __XJNI_CLASS_H__

#include <jni.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XJNI_CLASS_FREE_FLAG         0x01u
#define XJNI_CLASS_THREAD_SAFE_FLAG  0x02u

#define XJNI_CLASS_SHOULD_FREE(ctx) \
    ((ctx) != NULL && (((ctx)->flags & XJNI_CLASS_FREE_FLAG) != 0))

#define XJNI_CLASS_SHOULD_THREAD(ctx) \
    ((ctx) != NULL && (((ctx)->flags & XJNI_CLASS_THREAD_SAFE_FLAG) != 0))

/**
 * @brief Allocate/construct a native C++ object.
 *
 * The callback MUST construct the C++ object correctly.
 *
 * Example:
 *
 *     return new MyClass();
 *
 * @param env JNI environment.
 * @param data Optional native initial value.
 * @param size Size/hint associated with data.
 *
 * @return Native object or NULL.
 */
typedef void* (*xjni_class_alloc_t)(JNIEnv* env,const void* data,size_t size);

/**
 * @brief Allocate a Java object.
 *
 * @param env JNI environment.
 * @param size Native object size/hint.
 *
 * @return Java object or NULL.
 */
typedef jobject (*xjni_class_jalloc_t)(JNIEnv* env,size_t size);

/**
 * @brief Copy native C++ object -> Java object.
 *
 * @param env JNI environment.
 * @param in Native object.
 * @param out Java object.
 */
typedef void (*xjni_class_set_t)(JNIEnv* env,const void* in,jobject out);

/**
 * @brief Copy Java object -> native C++ object.
 *
 * This callback should normally allocate/construct a new C++ object.
 *
 * @param env JNI environment.
 * @param in Java object.
 *
 * @return Native object or NULL.
 */
typedef void* (*xjni_class_get_t)(JNIEnv* env,jobject in);

/**
 * @brief Destroy a native C++ object.
 *
 * Example:
 *
 *     delete static_cast<MyClass*>(ptr);
 */
typedef void (*xjni_class_free_t)(void* ptr);

/* ============================================================= */
/* Context                                                       */
/* ============================================================= */

typedef struct xjni_class_ctx_t {

    xjni_class_set_t   set;
    xjni_class_get_t   get;
    xjni_class_free_t  freef;
    xjni_class_alloc_t alloc;
    xjni_class_jalloc_t jalloc;

    uint32_t flags;

} xjni_class_ctx_t;

/* ============================================================= */
/* Context API                                                    */
/* ============================================================= */

/**
 * @brief Initialize context.
 */
JNIEXPORT void JNICALL
xjni_class_ctx_init(
    xjni_class_ctx_t* ctx,
    xjni_class_set_t set,
    xjni_class_get_t get,
    xjni_class_free_t freef,
    xjni_class_alloc_t alloc,
    xjni_class_jalloc_t jalloc,
    uint32_t flags
);

/* ============================================================= */
/* Object API                                                     */
/* ============================================================= */

/**
 * @brief Create Java object from native C++ object.
 *
 * Native object is temporary and is released after set().
 */
JNIEXPORT jobject JNICALL
xjni_class_new(xjni_class_ctx_t* ctx,JNIEnv* env,const void* data,size_t size);

/**
 * @brief Convert Java object -> native C++ object.
 *
 * Returned object belongs to caller.
 */
JNIEXPORT void* JNICALL
xjni_class_get(xjni_class_ctx_t* ctx,JNIEnv* env,jobject obj);

/**
 * @brief Release native C++ object.
 *
 * mode == 0:
 *     set(native, java)
 *     free(native)
 *
 * mode == JNI_COMMIT:
 *     set(native, java)
 *     keep(native)
 *
 * mode == JNI_ABORT:
 *     discard native changes
 *     free(native)
 */
JNIEXPORT void JNICALL
xjni_class_release(xjni_class_ctx_t* ctx,JNIEnv* env,jobject obj,void* object,jint mode);

/* ============================================================= */
/* Region API                                                     */
/* ============================================================= */

/**
 * @brief Java object -> C++ object -> raw buffer.
 *
 * The buffer receives exactly `size` bytes from the native object.
 *
 * This is intended for classes whose native representation is safely
 * copyable into the supplied buffer.
 */
JNIEXPORT void JNICALL
xjni_class_get_region(xjni_class_ctx_t* ctx,JNIEnv* env,jobject obj,size_t size,void* buf);

/**
 * @brief Buffer -> C++ object -> Java object.
 *
 * `data` is optional initial data passed to alloc().
 */
JNIEXPORT void JNICALL
xjni_class_set_region(xjni_class_ctx_t* ctx,JNIEnv* env,jobject obj,const void* data,size_t size,const void* buf);

static inline void xjni_class_free_null(void* ptr) { (void)ptr; }

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_CLASS_H__ */
