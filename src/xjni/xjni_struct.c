#include "base-jni.h"

#include <xjni_thread.h>
#include <xjni_log.h>
#include <xjni_string.h>
#include <xjni_struct.h>

#define XJNI_LOCK(ctx) if (XJNI_CTX_SHOULD_THREAD(ctx)) xjni_mutex_lock(&ctx->mutex)
#define XJNI_UNLOCK(ctx) if (XJNI_CTX_SHOULD_THREAD(ctx)) xjni_mutex_unlock(&ctx->mutex)

JNIEXPORTC void JNICALL xjni_struct_ctx_init(xjni_struct_ctx_t* ctx,xjni_struct_set_t set,
					     xjni_struct_get_t get,xjni_struct_free_t freef,
					     xjni_struct_alloc_t alloc,xjni_struct_jalloc_t jalloc,uint32_t flags) {
	if (ctx == NULL) return;
	if (flags & XJNI_STRUCT_THREAD_SAFE_FLAG) xjni_mutex_init(&ctx->mutex);
	ctx->set = set;
	ctx->get = get;
	ctx->freef = freef;
	ctx->alloc = alloc;
	ctx->jalloc = jalloc;
	ctx->flags = flags;
}

JNIEXPORTC jobject JNICALL xjni_struct_new(xjni_struct_ctx_t* ctx, JNIEnv *env, void* data, size_t size) {
	if (!ctx || !env || size == 0) return NULL;
	if (!ctx->alloc || !ctx->set) return NULL;
	XJNI_LOCK(ctx);
	void *ptr = ctx->alloc(env, data, size);
	if (!ptr) {
		XJNI_LOGE("xjni_struct_new", "Native allocation failed (size=%zu)", size);
		XJNI_UNLOCK(ctx);
		return NULL;
	}
	if (!ctx->jalloc) {
		XJNI_LOGE("xjni_struct_new", "Java allocator (jalloc) not set");
		ctx->freef(ptr);
		XJNI_UNLOCK(ctx);
		return NULL;
	}
	jobject obj = ctx->jalloc(env, size);
	if (!obj) {
		XJNI_LOGE("xjni_struct_new", "Java object allocation failed");
		ctx->freef(ptr);
		XJNI_UNLOCK(ctx);
		return NULL;
	}
	ctx->set(env, ptr, obj);
	if (_ExceptionCheck(env)) {
		ctx->freef(ptr);
		_DeleteLocalRef(env, obj);
		XJNI_UNLOCK(ctx);
		return NULL;
	}
	XJNI_UNLOCK(ctx);
	return obj;
}

JNIEXPORTC void* JNICALL xjni_struct_get(xjni_struct_ctx_t* ctx,JNIEnv *env, jobject obj) {
	XJNI_LOCK(ctx);
	if (!ctx->get) {
		XJNI_UNLOCK(ctx);
		return NULL;
	}
	void* ret = ctx->get(env, obj);
	XJNI_UNLOCK(ctx);
	return ret;
}

JNIEXPORTC void JNICALL xjni_struct_release(xjni_struct_ctx_t* ctx,JNIEnv *env,jobject obj,void* elements,jint mode) {
	if (!elements) return;
	XJNI_LOCK(ctx);
	if ((mode == 0 || mode == JNI_COMMIT) && ctx->set)	ctx->set(env, elements, obj);
	if ((mode == 0 || mode == JNI_ABORT) && ctx->freef)	ctx->freef(elements);
	XJNI_UNLOCK(ctx);
}

JNIEXPORTC void JNICALL xjni_struct_get_region(xjni_struct_ctx_t* ctx,JNIEnv *env, jobject obj, size_t size, void *buf) {
	if (!env || !obj || !buf || size == 0) return;
	XJNI_LOCK(ctx);
	void* ptr = ctx->get(env, obj);
	if (!ptr) {
		XJNI_UNLOCK(ctx);
		return;
	}
	jmemcpy(buf, ptr, size);
	if (XJNI_CTX_SHOULD_FREE(ctx) && ctx->freef) ctx->freef(ptr);
	XJNI_UNLOCK(ctx);
}

JNIEXPORTC void JNICALL xjni_struct_set_region(xjni_struct_ctx_t* ctx,JNIEnv *env, jobject obj, void* data, size_t size, const void *buf) {
	const jboolean UseThread = XJNI_CTX_SHOULD_THREAD(ctx);
	if (!env || !obj || !buf || size == 0) return;
	XJNI_LOCK(ctx);
	void* ptr = NULL;
	if (ctx->alloc) ptr = ctx->alloc(env,data,size);
	else {
		XJNI_UNLOCK(ctx);
		return;
	}
	if (!ptr) {
		XJNI_UNLOCK(ctx);
		return;
	}
	jmemcpy(ptr, buf, size);
	ctx->set(env, ptr, obj);
	if (!_ExceptionCheck(env)) ctx->freef(ptr);
	else ctx->freef(ptr);

	XJNI_UNLOCK(ctx);
}
