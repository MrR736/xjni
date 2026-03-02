#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni_utils.h>
#include <xjni_classcache.h>

static jclass classClass = NULL;
static jmethodID getNameMethod = NULL;

JNIEXPORTC jboolean JNICALL IsSameObjectChars(JNIEnv *env, const char* className, jobject obj2) {
	if (obj2 == NULL || className == NULL) return JNI_FALSE;
	jclass cls = _FindClass(env, className);
	if (cls == NULL) return JNI_FALSE;
	jboolean result = _IsInstanceOf(env, obj2, cls);
	return result;
}

JNIEXPORTC jstring JNICALL GetObjectName(JNIEnv* env, jobject obj) {
	if (obj == NULL) return NULL;
	if (classClass == NULL) {
		jclass localClass = _FindClass(env, "java/lang/Class");
		classClass = _NewGlobalRef(env, localClass);
		_DeleteLocalRef(env, localClass);
		getNameMethod = _GetMethodID(env,classClass,"getName","()Ljava/lang/String;");
	}
	jclass objClass = _GetObjectClass(env, obj);
	jstring name = (jstring)_CallObjectMethod(env,objClass,getNameMethod);
	_DeleteLocalRef(env, objClass);
	return name;
}

JNIEXPORTC char* JNICALL GetObjectNameChars(JNIEnv* env, jobject obj) {
	if (obj == NULL) return NULL;
	jstring name = GetObjectName(env, obj);
	const char* temp = _GetStringUTFChars(env, name, NULL);
	char* result = strdup(temp);
	_ReleaseStringUTFChars(env, name, temp);
	return result;
}

JNIEXPORTC jfieldID JNICALL GetFieldIDByName(JNIEnv* env,const char* className,const char* fieldName,const char* sig) {
	if (!env || !className || !fieldName || !sig) return NULL;
	jclass cls = xjni_classcache_get(env,className);
	if (!cls) {
		jclass localCls = _FindClass(env,className);
		if (!localCls) return NULL;
		cls = xjni_classcache_insert(env,className,localCls);
		if (!cls) return NULL;
	}
	jfieldID field = _GetFieldID(env,cls,fieldName,sig);
	if (!field) return NULL;
	return field;
}

JNIEXPORTC jfieldID JNICALL GetFieldIDByObject(JNIEnv* env,jobject obj,const char* fieldName,const char* sig) {
	if (!env || !obj || !fieldName || !sig) return NULL;
	char* className = GetObjectNameChars(env,obj);
	if (!className) return NULL;
	return GetFieldIDByName(env,className,fieldName,sig);
}

JNIEXPORTC jint JNICALL XJNI_Utily_OnLoad(JavaVM* vm,void* reserved,jint ver) {
	JNIEnv* env = NULL;
	if (_GetEnv(vm,(void**)&env,ver) != JNI_OK) return JNI_ERR;
	if (!xjni_classcache_init()) return JNI_ERR;
	return ver;
}

JNIEXPORTC void JNICALL XJNI_Utily_OnUnload(JavaVM* vm,void* reserved,jint ver) {
	JNIEnv* env = NULL;
	jint rs = _GetEnv(vm,(void**)&env,ver);
	if (rs == JNI_EDETACHED) {
		if (_AttachCurrentThread(vm,(void**)&env,NULL) != JNI_OK) return;
	} else if (rs != JNI_OK) return;
	xjni_classcache_clear(env);
	if (rs == JNI_EDETACHED) _DetachCurrentThread(vm);
}
