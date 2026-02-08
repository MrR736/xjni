#include <xjni_log.h>
#include <xjni_va_list.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

static jclass gStringCls = NULL;
static jmethodID gFormatMid = NULL;

static jboolean ensureStringFormat(JNIEnv *env) {
	if (gStringCls && gFormatMid) return JNI_TRUE;

	jclass local = _FindClass(env, "java/lang/String");
	if (!local) return JNI_FALSE;

	jclass global = _NewGlobalRef(env, local);
	_DeleteLocalRef(env, local);
	if (!global) return JNI_FALSE;

	jmethodID mid = _GetStaticMethodID(env, global,"format","(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;");
	if (!mid) {
		_DeleteGlobalRef(env, global);
		return JNI_FALSE;
	}
	/* publish atomically */
	gStringCls = global;
	gFormatMid = mid;
	return JNI_TRUE;
}

JNIEXPORTC void JNICALL JSnPrintf(JNIEnv *env, char *s, size_t maxlen,jstring format, jargs_t args) {
	jstring result = NULL;
	const char *utf = NULL;
	if (!ensureStringFormat(env)) {
		XJNI_LOGE("XJniVaList", "String.format init failed");
		return;
	}
	if (args) result = (jstring)_CallStaticObjectMethod(env, gStringCls, gFormatMid, format, args);
	else result = (jstring)_CallStaticObjectMethod(env, gStringCls, gFormatMid, format);
	if (_ExceptionCheck(env) || !result) {
		XJNI_LOGE("XJniVaList", "String.format threw");
		_ExceptionClear(env);
		goto cleanup;
	}
	utf = _GetStringUTFChars(env, result, NULL);
	if (!utf) {
		XJNI_LOGE("XJniVaList", "GetStringUTFChars failed");
		goto cleanup;
	}
	snprintf(s,maxlen,"%s",utf);
cleanup:
	if (utf) _ReleaseStringUTFChars(env, result, utf);
	if (result) _DeleteLocalRef(env, result);
}

JNIEXPORTC void JNICALL JSPrintf(JNIEnv *env,char* s,jstring format,jargs_t args) {
	jstring result = NULL;
	const char *utf = NULL;
	if (!ensureStringFormat(env)) {
		XJNI_LOGE("XJniVaList", "String.format init failed");
		return;
	}
	if (args) result = (jstring)_CallStaticObjectMethod(env, gStringCls, gFormatMid, format, args);
	else result = (jstring)_CallStaticObjectMethod(env, gStringCls, gFormatMid, format);
	if (_ExceptionCheck(env) || !result) {
		XJNI_LOGE("XJniVaList", "String.format threw");
		_ExceptionClear(env);
		goto cleanup;
	}
	utf = _GetStringUTFChars(env, result, NULL);
	if (!utf) {
		XJNI_LOGE("XJniVaList", "GetStringUTFChars failed");
		goto cleanup;
	}
	sprintf(s,"%s",utf);
cleanup:
	if (utf) _ReleaseStringUTFChars(env, result, utf);
	if (result) _DeleteLocalRef(env, result);
}

JNIEXPORTC void JNICALL JSnPrintfUTF(JNIEnv *env,char* s,size_t maxlen,const char* format,jargs_t args) {
	if (format == NULL) return;
	jstring jstr = _NewStringUTF(env,format);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	JSnPrintf(env,s,maxlen,jstr,args);
	_DeleteLocalRef(env,jstr);
}

JNIEXPORTC void JNICALL JSPrintfUTF(JNIEnv *env,char* s,const char* format,jargs_t args) {
	JSnPrintfUTF(env,s,SIZE_MAX,format,args);
}

JNIEXPORTC void JNICALL JFPrintf(JNIEnv *env,FILE* fp,jstring format, jargs_t args) {
	jstring result = NULL;
	const char *utf = NULL;
	if (!ensureStringFormat(env)) {
		XJNI_LOGE("XJniVaList", "String.format init failed");
		return;
	}
	if (args) result = (jstring)_CallStaticObjectMethod(env, gStringCls, gFormatMid, format, args);
	else result = (jstring)_CallStaticObjectMethod(env, gStringCls, gFormatMid, format);
	if (_ExceptionCheck(env) || !result) {
		XJNI_LOGE("XJniVaList", "String.format threw");
		_ExceptionClear(env);
		goto cleanup;
	}
	utf = _GetStringUTFChars(env, result, NULL);
	if (!utf) {
		XJNI_LOGE("XJniVaList", "GetStringUTFChars failed");
		goto cleanup;
	}
	fprintf(fp,"%s",utf);
cleanup:
	if (utf) _ReleaseStringUTFChars(env, result, utf);
	if (result) _DeleteLocalRef(env, result);
}

JNIEXPORTC void JNICALL JFPrintfUTF(JNIEnv *env,FILE* fp,const char* format,jargs_t args) {
	if (format == NULL) return;
	jstring jstr = _NewStringUTF(env,format);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	JFPrintf(env,fp,jstr,args);
	_DeleteLocalRef(env,jstr);
}


JNIEXPORTC void JNICALL JPrintf(JNIEnv *env,jstring format, jargs_t args) {
	JFPrintf(env,stdout,format,args);
}

JNIEXPORTC void JNICALL JPrintfUTF(JNIEnv *env,const char* format, jargs_t args) {
	JFPrintfUTF(env,stdout,format,args);
}

JNIEXPORTC void JNICALL JDPrintf(JNIEnv *env,int fd,jstring format, jargs_t args) {
	jstring result = NULL;
	const char *utf = NULL;
	if (!ensureStringFormat(env)) {
		XJNI_LOGE("XJniVaList", "String.format init failed");
		return;
	}
	if (args) result = (jstring)_CallStaticObjectMethod(env, gStringCls, gFormatMid, format, args);
	else result = (jstring)_CallStaticObjectMethod(env, gStringCls, gFormatMid, format);
	if (_ExceptionCheck(env) || !result) {
		XJNI_LOGE("XJniVaList", "String.format threw");
		_ExceptionClear(env);
		goto cleanup;
	}
	utf = _GetStringUTFChars(env, result, NULL);
	if (!utf) {
		XJNI_LOGE("XJniVaList", "GetStringUTFChars failed");
		goto cleanup;
	}
#ifdef _WIN32
	FILE* fp = fdopen(fd, "w");
	if (fp) {
		fprintf(fp,"%s",utf);
		fflush(fp);
	} else {
		XJNI_LOGE("XJniVaList","_fdopen failed for fd %d", fd);
		goto cleanup;
	}
#else
	dprintf(fd,"%s",utf);
#endif
cleanup:
	if (utf) _ReleaseStringUTFChars(env, result, utf);
	if (result) _DeleteLocalRef(env, result);
}

JNIEXPORTC void JNICALL JDPrintfUTF(JNIEnv *env,int fd,const char* format,jargs_t args) {
	if (format == NULL) return;
	jstring jstr = _NewStringUTF(env,format);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	JDPrintf(env,fd,jstr,args);
	_DeleteLocalRef(env,jstr);
}
