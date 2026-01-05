#include <stdarg.h>
#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

static JavaVM* g_vm = NULL;
static jobject g_classLoader = NULL;
static jmethodID g_loadClass = NULL;

JNIEXPORTC jobjectArray JNICALL xjni_NewObjectArray(JNIEnv* env,jclass clz,jsize len) {
	if (clz == NULL || len < 0) return NULL;
	jobjectArray a = _NewObjectArray(env,len,clz,NULL);
	if (a == NULL) return NULL;
	return a;
}

JNIEXPORTC jbyteArray JNICALL xjni_NewByteArray(JNIEnv* env,const jbyte* data,jsize len) {
	if (!env || len < 0) return NULL;
	jbyteArray arr = _NewByteArray(env,len);
	if (!arr) return NULL;
	if (data && len > 0) _SetByteArrayRegion(env,arr,0,len,data);
	return arr;
}

JNIEXPORTC jintArray JNICALL xjni_NewIntArray(JNIEnv* env,const jint* data,jsize len) {
	if (!env || len < 0) return NULL;
	jintArray arr = _NewIntArray(env,len);
	if (!arr) return NULL;
	if (data && len > 0) _SetIntArrayRegion(env,arr,0,len,data);
	return arr;
}

JNIEXPORTC jlongArray JNICALL xjni_NewLongArray(JNIEnv* env,const jlong* data,jsize len) {
	if (!env || len < 0) return NULL;
	jlongArray arr = _NewLongArray(env,len);
	if (!arr) return NULL;
	if (data && len > 0) _SetLongArrayRegion(env,arr,0,len,data);
	return arr;
}

JNIEXPORTC jfloatArray JNICALL xjni_NewFloatArray(JNIEnv* env,const float* data,jsize len) {
	if (!env || len < 0) return NULL;
	jfloatArray arr = _NewFloatArray(env,len);
	if (!arr) return NULL;
	if (data && len > 0) _SetFloatArrayRegion(env,arr,0,len,data);
	return arr;
}

JNIEXPORTC jdoubleArray JNICALL xjni_NewDoubleArray(JNIEnv* env,const double* data,jsize len) {
	if (!env || len < 0) return NULL;
	jdoubleArray arr = _NewDoubleArray(env,len);
	if (!arr) return NULL;
	if (data && len > 0) _SetDoubleArrayRegion(env,arr,0,len,data);
	return arr;
}

JNIEXPORTC jint JNICALL XJNI_New_OnLoad(JavaVM* vm,void* reserved,jint ver) {
	JNIEnv* env = NULL;
	if (_GetEnv(vm,(void**)&env,ver) != JNI_OK)
		return JNI_ERR;

	g_vm = vm;

	jclass clsThread = _FindClass(env,"java/lang/Thread");
	jmethodID midCurrent = _GetStaticMethodID(
		env,clsThread,"currentThread","()Ljava/lang/Thread;");
	jmethodID midGetCL = _GetMethodID(
		env,clsThread,"getContextClassLoader","()Ljava/lang/ClassLoader;");

	jobject thread = _CallStaticObjectMethod(env,clsThread,midCurrent);
	jobject loader = _CallObjectMethod(env,thread,midGetCL);

	g_classLoader = _NewGlobalRef(env,loader);

	jclass clsCL = _FindClass(env,"java/lang/ClassLoader");
	g_loadClass = _GetMethodID(
		env,clsCL,"loadClass","(Ljava/lang/String;)Ljava/lang/Class;");

	_DeleteLocalRef(env,clsThread);
	_DeleteLocalRef(env,clsCL);
	_DeleteLocalRef(env,thread);
	_DeleteLocalRef(env,loader);

	return ver;
}

JNIEXPORTC void JNICALL XJNI_New_OnUnload(JavaVM* vm,void* reserved,jint ver) {
	JNIEnv* env = NULL;

	if (_GetEnv(vm,(void**)&env,ver) != JNI_OK)
		return;

	if (g_classLoader) {
		_DeleteGlobalRef(env,g_classLoader);
		g_classLoader = NULL;
	}

	g_loadClass = NULL;
	g_vm = NULL;
}


static jclass FindClassSafe(JNIEnv* env,const char* name) {
	jclass cls = _FindClass(env,name);
	if (cls)
		return cls;

	_ExceptionClear(env);

	jstring jname = _NewStringUTF(env,name);
	cls = (jclass)_CallObjectMethod_l(env,g_classLoader,g_loadClass,jname);

	_DeleteLocalRef(env,jname);
	return cls;
}

JNIEXPORTC jobject JNICALL NewObjectBuilderV(JNIEnv* env,const char* className,const char* sig,va_list ap) {
	if (!env || !className || !sig)
		return NULL;

	jclass cls = FindClassSafe(env,className);
	if (!cls)
		return NULL;

	jmethodID ctor = _GetMethodID(env,cls,"<init>",sig);
	if (!ctor) {
		_DeleteLocalRef(env,cls);
		return NULL;
	}

	va_list args;
	va_copy(args,ap);
	jobject obj = _NewObjectV(env,cls,ctor,args);
	va_end(args);

	_DeleteLocalRef(env,cls);
	return obj;
}

JNIEXPORTC jobject JNICALL NewObjectBuilder(JNIEnv* env,const char* className,const char* sig,...) {
	va_list ap;
	va_start(ap,sig);
	jobject obj = NewObjectBuilderV(env,className,sig,ap);
	va_end(ap);
	return obj;
}

