/**
 * @file xjni_struct.h
 * @brief XJNI Struct Mapping Utilities
 * @date 2026
 * @author MrR736
 * @license GPL-3.0
 *
 * @details
 * The XJNI Struct module provides a generic, callback-based interface for
 * mapping native C structs to Java objects through JNI.
 *
 * It enables safe and flexible bidirectional data transfer between native
 * memory and Java objects while maintaining clear ownership semantics.
 *
 * This design:
 *  - Avoids global state
 *  - Supports multiple struct types
 *  - Is thread-safe when contexts are not shared mutably
 *  - Mirrors JNI array access semantics for commit/abort behavior
 *
 * ---------------------------------------------------------------------------
 * Memory Ownership Model
 * ---------------------------------------------------------------------------
 *
 * - Memory returned by xjni_struct_get() is owned by the caller.
 * - The caller MUST call xjni_struct_release() on the returned pointer.
 * - Allocation and deallocation behavior depends entirely on the
 *   user-provided callbacks in xjni_struct_ctx_t.
 * - The library itself does not assume malloc/free unless specified
 *   in the callbacks.
 *
 * ---------------------------------------------------------------------------
 * Typical Usage
 * ---------------------------------------------------------------------------
 *
 * @code
 * xjni_struct_ctx_t ctx;
 *
 * xjni_struct_ctx_init(&ctx,
 *                      my_set_callback,
 *                      my_get_callback,
 *                      my_free_callback,
 *                      my_alloc_callback,
 *                      my_jalloc_callback,0);
 *
 * jobject obj = xjni_struct_new(&ctx, env, NULL, sizeof(MyStruct));
 *
 * MyStruct* data = (MyStruct*) xjni_struct_get(&ctx, env, obj);
 *
 * data->field = 42;
 *
 * xjni_struct_release(&ctx, env, obj, data, JNI_COMMIT);
 * @endcode
 *
 * ---------------------------------------------------------------------------
 */

#ifndef __XJNI_STRUCT_H__
#define __XJNI_STRUCT_H__

#include <stdint.h>
#include <jni.h>
#include <xjni_thread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup XJNI_Struct XJNI Struct Utilities
 * @brief Callback-based native struct ↔ Java object bridge.
 *
 * Provides mechanisms to:
 *  - Allocate native struct memory
 *  - Allocate corresponding Java objects
 *  - Extract struct data from Java objects
 *  - Commit native changes back to Java
 *  - Release temporary native memory safely
 *
 * @{
 */

/* ============================================================= */
/* Callback Types                                                */
/* ============================================================= */

/**
 * @brief Allocate native memory for a struct.
 *
 * @param env JNI environment pointer.
 * @param data Optional initial data (may be NULL).
 * @param size Size of memory to allocate in bytes.
 *
 * @return Pointer to allocated memory, or NULL on failure.
 */
typedef void* (*xjni_struct_alloc_t)(JNIEnv *env, void* data, size_t size);

/**
 * @brief Allocate a new Java object associated with a struct.
 *
 * @param env JNI environment pointer.
 * @param size Optional hint indicating native struct size.
 *
 * @return New Java object, or NULL on failure.
 */
typedef jobject (*xjni_struct_jalloc_t)(JNIEnv *env, size_t size);

/**
 * @brief Commit native struct data into a Java object.
 *
 * @param env JNI environment pointer.
 * @param in Pointer to native struct.
 * @param out Target Java object.
 */
typedef void (*xjni_struct_set_t)(JNIEnv *env, void* in, jobject out);

/**
 * @brief Retrieve native struct data from a Java object.
 *
 * @param env JNI environment pointer.
 * @param in Java object containing struct state.
 *
 * @return Pointer to native struct copy.
 */
typedef void* (*xjni_struct_get_t)(JNIEnv *env, jobject in);

/**
 * @brief Free temporary native struct memory.
 *
 * @param ptr Pointer to memory to release.
 */
typedef void (*xjni_struct_free_t)(void*);

/**
 * @brief Flag indicating that temporary native memory should be automatically freed.
 */
#define XJNI_STRUCT_FREE_FLAG       0x01  // auto-free temporary memory

/**
 * @brief Flag indicating that the struct context should use thread-safe operations.
 */
#define XJNI_STRUCT_THREAD_SAFE_FLAG 0x02 // enable thread-safe mode

/**
 * @brief Check if the context is configured to automatically free temporary memory.
 *
 * @param ctx Pointer to xjni_struct_ctx_t context.
 * @return Non-zero if automatic free is enabled, zero otherwise.
 *
 * @note Safe to call even if ctx is NULL.
 */
#define XJNI_CTX_SHOULD_FREE(ctx)  ((ctx) && ((ctx)->flags & XJNI_STRUCT_FREE_FLAG))

/**
 * @brief Check if the context is configured for thread-safe operations.
 *
 * @param ctx Pointer to xjni_struct_ctx_t context.
 * @return Non-zero if thread-safe mode is enabled, zero otherwise.
 *
 * @note Safe to call even if ctx is NULL.
 */
#define XJNI_CTX_SHOULD_THREAD(ctx)  ((ctx) && ((ctx)->flags & XJNI_STRUCT_THREAD_SAFE_FLAG))

/* ============================================================= */
/* Context Structure                                             */
/* ============================================================= */

/**
 * @brief Context describing struct ↔ Java mapping behavior.
 *
 * This context stores callback pointers and configuration flags used
 * by the XJNI struct API to safely map native memory to Java objects.
 *
 * It supports thread-safe operations when the appropriate flag is set.
 */
typedef struct {
	/** @brief Commit native struct data to a Java object. */
	xjni_struct_set_t	set;

	/** @brief Retrieve a native struct copy from a Java object. */
	xjni_struct_get_t	get;

	/** @brief Free temporary native memory returned by `get`. */
	xjni_struct_free_t	freef;

	/** @brief Allocate native memory for a struct. */
	xjni_struct_alloc_t	alloc;

	/** @brief Allocate a new Java object corresponding to a native struct. */
	xjni_struct_jalloc_t	jalloc;

	/** @brief Mutex used when thread-safe mode is enabled (`XJNI_STRUCT_THREAD_SAFE_FLAG`). */
	xjni_mutex_t		mutex;

	/** @brief Control flags for behavior:
	 *  - `XJNI_STRUCT_FREE_FLAG`       : Automatically free temporary memory.
	 *  - `XJNI_STRUCT_THREAD_SAFE_FLAG`: Enable thread-safe mode.
	 */
	uint32_t		flags;
} xjni_struct_ctx_t;

/* ============================================================= */
/* Public API                                                    */
/* ============================================================= */

/**
 * @brief Initialize a struct context.
 *
 * Stores the provided callback pointers into the context structure.
 * No validation is performed.
 *
 * @param ctx Context to initialize.
 * @param set Commit callback.
 * @param get Retrieval callback.
 * @param freef Free callback.
 * @param alloc Native allocation callback.
 * @param jalloc Java object allocation callback.
 * @param flags
 *
 * @warning Callbacks must not be NULL if the corresponding
 *          functionality will be used.
 */
JNIEXPORT void JNICALL xjni_struct_ctx_init(xjni_struct_ctx_t* ctx,xjni_struct_set_t set,xjni_struct_get_t get,
					    xjni_struct_free_t freef,xjni_struct_alloc_t alloc,xjni_struct_jalloc_t jalloc,uint32_t flags);

/**
 * @brief Create a new Java object backed by native struct memory.
 *
 * Allocates native memory using ctx->alloc and a Java object using
 * ctx->jalloc, then commits the data using ctx->set.
 *
 * @param ctx Struct context.
 * @param env JNI environment pointer.
 * @param data Optional initial data (may be NULL).
 * @param size Size of struct in bytes.
 *
 * @return Newly created Java object, or NULL on failure.
 */
JNIEXPORT jobject JNICALL xjni_struct_new(xjni_struct_ctx_t* ctx,JNIEnv *env,void* data,size_t size);

/**
 * @brief Retrieve a native struct copy from a Java object.
 *
 * The returned memory is owned by the caller and MUST be
 * released via xjni_struct_release().
 *
 * @param ctx Struct context.
 * @param env JNI environment pointer.
 * @param obj Java object.
 *
 * @return Pointer to native struct copy, or NULL on failure.
 */
JNIEXPORT void* JNICALL xjni_struct_get(xjni_struct_ctx_t* ctx,JNIEnv *env,jobject obj);

/**
 * @brief Release previously retrieved struct memory.
 *
 * Behavior mirrors JNI array release semantics.
 *
 * @param ctx Struct context.
 * @param env JNI environment pointer.
 * @param obj Associated Java object.
 * @param elements Pointer returned by xjni_struct_get().
 * @param mode Release mode:
 *  - 0: Commit changes and free memory.
 *  - JNI_COMMIT: Commit changes only.
 *  - JNI_ABORT: Discard changes and free memory.
 */
JNIEXPORT void JNICALL xjni_struct_release(xjni_struct_ctx_t* ctx,JNIEnv *env,jobject obj,void* elements,jint mode);

/**
 * @brief Copy a region of struct data into a buffer.
 *
 * Temporary memory is automatically released.
 *
 * @param ctx Struct context.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param size Number of bytes to copy.
 * @param buf Destination buffer.
 */
JNIEXPORT void JNICALL xjni_struct_get_region(xjni_struct_ctx_t* ctx,JNIEnv *env,jobject obj,size_t size,void *buf);

/**
 * @brief Update struct data from a buffer and commit to Java.
 *
 * Temporary memory is automatically released.
 *
 * @param ctx Struct context.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param data Optional initial native struct data (may be NULL).
 * @param size Number of bytes to copy.
 * @param buf Source buffer.
 */
JNIEXPORT void JNICALL xjni_struct_set_region(xjni_struct_ctx_t* ctx,JNIEnv *env,jobject obj,
					      void* data,size_t size,const void *buf);


/**
 * @brief No-op free helper.
 *
 * Provides a compatible free callback that performs no action.
 *
 * Useful when native memory must not be released.
 *
 * @param ptr Pointer to memory (ignored).
 */
static inline void xjni_struct_free_null(void* ptr) {
	(void)ptr;
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRUCT_H__ */
