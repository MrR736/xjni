#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "base-jni.h"

#include <xjni_utils.h>
#include <xjni_classcache.h>

static jclass classClass = NULL;
static jmethodID getNameMethod = NULL;

JNIEXPORTC jboolean JNICALL IsSameObjectChars(JNIEnv* env, const char* className, jobject obj2) {
	if (!env || !className || !obj2) return JNI_FALSE;
	jclass cls = _FindClass(env, className);
	if (!cls) return JNI_FALSE;
	jboolean result = _IsInstanceOf(env, obj2, cls);
	_DeleteLocalRef(env, cls);
	return result;
}

JNIEXPORTC jstring JNICALL GetObjectName(JNIEnv* env, jobject obj) {
	if (!env || !obj) return NULL;
	if (!classClass) {
		jclass localClass = _FindClass(env, "java/lang/Class");
		if (!localClass) return NULL;
		classClass = (jclass)_NewGlobalRef(env, localClass);
		_DeleteLocalRef(env, localClass);
		if (!classClass) return NULL;
		getNameMethod = _GetMethodID(env,classClass,"getName","()Ljava/lang/String;");
		if (!getNameMethod) {
			_DeleteGlobalRef(env, classClass);
			classClass = NULL;
			return NULL;
		}
	}
	jclass objClass = _GetObjectClass(env, obj);
	if (!objClass) return NULL;
	jstring name = (jstring)_CallObjectMethod(env,objClass,getNameMethod);
	_DeleteLocalRef(env, objClass);
	return name;
}

JNIEXPORTC char* JNICALL GetObjectNameChars(JNIEnv* env, jobject obj) {
	if (!env || !obj) return NULL;
	jstring name = GetObjectName(env, obj);
	if (!name) return NULL;
	const char* temp = _GetStringUTFChars(env, name, NULL);
	if (!temp) {
		_DeleteLocalRef(env, name);
		return NULL;
	}
	char* result = strdup(temp);
	_ReleaseStringUTFChars(env, name, temp);
	_DeleteLocalRef(env, name);
	return result;
}

JNIEXPORTC jfieldID JNICALL GetFieldIDByName(JNIEnv* env,const char* className,const char* fieldName,const char* sig) {
	if (!env || !className || !fieldName || !sig) return NULL;
	jclass cls = xjni_classcache_get(env, className);
	if (!cls) {
		jclass localCls = _FindClass(env, className);
		if (!localCls) return NULL;
		cls = xjni_classcache_insert(env,className,localCls);
		_DeleteLocalRef(env, localCls);
		if (!cls) return NULL;
	}
	return _GetFieldID(env,cls,fieldName,sig);
}

JNIEXPORTC jfieldID JNICALL GetFieldIDByObject(JNIEnv* env,jobject obj,const char* fieldName,const char* sig) {
	if (!env || !obj || !fieldName || !sig) return NULL;
	char* className = GetObjectNameChars(env, obj);
	if (!className) return NULL;
	jfieldID field =
	GetFieldIDByName(env,className,fieldName,sig);
	free(className);
	return field;
}

JNIEXPORTC jint JNICALL XJNI_Utily_OnLoad(JavaVM* vm,void* reserved,jint ver) {
	(void)reserved;
	if (!vm) return JNI_ERR;
	JNIEnv* env = NULL;
	if (_GetEnv(vm, (void**)&env, ver) != JNI_OK) return JNI_ERR;
	if (!xjni_classcache_init()) return JNI_ERR;
	return ver;
}

JNIEXPORTC void JNICALL XJNI_Utily_OnUnload(JavaVM* vm,void* reserved,jint ver) {
	(void)reserved;
	if (!vm) return;
	JNIEnv* env = NULL;
	jint rs = _GetEnv(vm,(void**)&env,ver);
	jboolean attached = JNI_FALSE;
	if (rs == JNI_EDETACHED) {
		if (_AttachCurrentThread(vm,(void**)&env,NULL) != JNI_OK) return;
		attached = JNI_TRUE;
	} else if (rs != JNI_OK) return;
	xjni_classcache_clear(env);
	if (classClass) {
		_DeleteGlobalRef(env, classClass);
		classClass = NULL;
	}
	getNameMethod = NULL;
	if (attached) _DetachCurrentThread(vm);
}
