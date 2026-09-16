/**
 * @file xjni_class.cpp
 * @brief XJNI Java Object <-> C++ Class Mapping Utilities
 * @date 2026
 * @author MrR736
 * @license MIT
 *
 * Generic callback-based Java object <-> native C++ object bridge.
 */

#include "base-jni.h"
#include "xjni_class.h"

#include <cstring>

static inline bool xjni_class_valid(const xjni_class_ctx_t* ctx) {
	return ctx != nullptr;
}

static inline bool xjni_class_exception(JNIEnv* env) {
	return env != nullptr && env->ExceptionCheck() == JNI_TRUE;
}

static inline void xjni_class_free(xjni_class_ctx_t* ctx,void* object) {
	if (object != nullptr && ctx != nullptr && ctx->freef != nullptr) {
		ctx->freef(object);
	}
}

JNIEXPORTC void JNICALL
xjni_class_ctx_init(
	xjni_class_ctx_t* ctx,xjni_class_set_t set,xjni_class_get_t get,
	xjni_class_free_t freef,xjni_class_alloc_t alloc,xjni_class_jalloc_t jalloc,
	uint32_t flags)
{
	if (ctx == nullptr) return;
	ctx->set    = set;
	ctx->get    = get;
	ctx->freef  = freef;
	ctx->alloc  = alloc;
	ctx->jalloc = jalloc;
	ctx->flags  = flags;
}

JNIEXPORTC jobject JNICALL
xjni_class_new(xjni_class_ctx_t* ctx,JNIEnv* env,const void* data,size_t size) {
	if (!xjni_class_valid(ctx) || env == nullptr) {
		return nullptr;
	}

	if (ctx->alloc == nullptr ||
		ctx->jalloc == nullptr ||
		ctx->set == nullptr) {
		return nullptr;
	}

	void* object =
	ctx->alloc(env,data,size);
	if (object == nullptr) return nullptr;

	if (xjni_class_exception(env)) {
		xjni_class_free(ctx, object);
		return nullptr;
	}

	jobject result =
	ctx->jalloc(env,size);
	if (result == nullptr) {
		xjni_class_free(ctx, object);
		return nullptr;
	}

	if (xjni_class_exception(env)) {
		env->DeleteLocalRef(result);
		xjni_class_free(ctx, object);
		return nullptr;
	}

	ctx->set(env,object,result);
	if (xjni_class_exception(env)) {
		env->DeleteLocalRef(result);
		xjni_class_free(ctx, object);
		return nullptr;
	}

	xjni_class_free(ctx, object);
	return result;
}

JNIEXPORTC void* JNICALL
xjni_class_get(xjni_class_ctx_t* ctx,JNIEnv* env,jobject obj) {
	if (!xjni_class_valid(ctx) || env == nullptr || obj == nullptr || ctx->get == nullptr) {
		return nullptr;
	}
	void* object = ctx->get(env,obj);
	if (xjni_class_exception(env)) {
		xjni_class_free(ctx, object);
		return nullptr;
	}
	return object;
}

JNIEXPORTC void JNICALL
xjni_class_release(xjni_class_ctx_t* ctx,JNIEnv* env,jobject obj,void* object,jint mode) {
	if (!xjni_class_valid(ctx) || env == nullptr || object == nullptr) {
		return;
	}

	if (mode == JNI_ABORT) {
		xjni_class_free(ctx, object);
		return;
	}

	if (ctx->set != nullptr && obj != nullptr) {
		ctx->set(env,object,obj);
	}
	if (mode == JNI_COMMIT) {
		return;
	}
	xjni_class_free(ctx, object);
}

JNIEXPORTC void JNICALL
xjni_class_get_region(xjni_class_ctx_t* ctx,JNIEnv* env,jobject obj,size_t size,void* buf) {
	if (!xjni_class_valid(ctx) || env == nullptr || obj == nullptr || buf == nullptr || size == 0) {
		return;
	}
	void* object = xjni_class_get(ctx,env,obj);
	if (object == nullptr) return;
	std::memcpy(buf,object,size);
	xjni_class_free(ctx,object);
}

JNIEXPORTC void JNICALL
xjni_class_set_region(xjni_class_ctx_t* ctx,JNIEnv* env,jobject obj,const void* data,size_t size,const void* buf) {
	if (!xjni_class_valid(ctx) || env == nullptr || obj == nullptr || buf == nullptr || size == 0) {
		return;
	}
	if (ctx->alloc == nullptr || ctx->set == nullptr) {
		return;
	}
	void* object = ctx->alloc(env,data,size);
	if (object == nullptr) return;
	if (xjni_class_exception(env)) {
		xjni_class_free(ctx, object);
		return;
	}
	std::memcpy(object,buf,size);
	ctx->set(env,object,obj);
	xjni_class_free(ctx,object);
}
