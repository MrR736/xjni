#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

JNIEXPORTC jobjectArray JNICALL xjni_NewObjectArray(JNIEnv* env, jclass clz, jsize len) {
	if (clz == NULL || len < 0) return NULL;
	jobjectArray a = _NewObjectArray(env, len, clz, NULL);
	if (a == NULL) return NULL;
	return a;
}

JNIEXPORTC jbyteArray JNICALL xjni_NewByteArray(JNIEnv* env, const jbyte* data, jsize len) {
	if (!env || len < 0) return NULL;
	jbyteArray arr = _NewByteArray(env, len);
	if (!arr) return NULL;
	if (data && len > 0) _SetByteArrayRegion(env, arr, 0, len, data);
	return arr;
}

JNIEXPORTC jintArray JNICALL xjni_NewIntArray(JNIEnv* env, const jint* data, jsize len) {
	if (!env || len < 0) return NULL;
	jintArray arr = _NewIntArray(env, len);
	if (!arr) return NULL;
	if (data && len > 0) _SetIntArrayRegion(env, arr, 0, len, data);
	return arr;
}

JNIEXPORTC jlongArray JNICALL xjni_NewLongArray(JNIEnv* env, const jlong* data, jsize len) {
	if (!env || len < 0) return NULL;
	jlongArray arr = _NewLongArray(env, len);
	if (!arr) return NULL;
	if (data && len > 0) _SetLongArrayRegion(env, arr, 0, len, data);
	return arr;
}

JNIEXPORTC jfloatArray JNICALL xjni_NewFloatArray(JNIEnv* env, const float* data, jsize len) {
	if (!env || len < 0) return NULL;
	jfloatArray arr = _NewFloatArray(env, len);
	if (!arr) return NULL;
	if (data && len > 0) _SetFloatArrayRegion(env, arr, 0, len, data);
	return arr;
}

JNIEXPORTC jdoubleArray JNICALL xjni_NewDoubleArray(JNIEnv* env, const double* data, jsize len) {
	if (!env || len < 0) return NULL;
	jdoubleArray arr = _NewDoubleArray(env, len);
	if (!arr) return NULL;
	if (data && len > 0) _SetDoubleArrayRegion(env, arr, 0, len, data);
	return arr;
}
