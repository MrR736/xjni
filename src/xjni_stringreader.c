#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <jni.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

// String Reader Utility
JNIEXPORTC jstringReader JNICALL NewStringReader(JNIEnv *env,jstring s) {
	jclass clz = _FindClass(env,"java/io/StringReader");
	if (!clz) {
		if (_ExceptionCheck(env)) {
			_ExceptionClear(env);
		}
		return NULL;
	}

	jmethodID ctor = _GetMethodID(env,clz,"<init>","(Ljava/lang/String;)V");
	if (!ctor) {
		if (_ExceptionCheck(env)) {
			_ExceptionClear(env);
		}
		_DeleteLocalRef(env,clz);
		return NULL;
	}

	jobject obj = _NewObject_l(env,clz,ctor,s);

	if (_ExceptionCheck(env)) {
		_ExceptionClear(env);
		obj = NULL;
	}

	_DeleteLocalRef(env,clz);
	return obj;
}

JNIEXPORTC jstringReader JNICALL NewStringReaderUTF(JNIEnv *env,const char* str) {
	if (str == NULL) return NULL;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return NULL;
	}
	jstringReader ret = NewStringReader(env,jstr);
	_DeleteLocalRef(env,jstr);
	return ret;
}

JNIEXPORTC jstring JNICALL StringReaderToString(JNIEnv *env,jstringReader sr) {
	if (env == NULL || sr == NULL) return NULL;

	jclass srClass = _GetObjectClass(env,sr);
	if (srClass == NULL) return NULL;

	jmethodID toStringMID = _GetMethodID(env,srClass,"toString","()Ljava/lang/String;");
	if (toStringMID == NULL) {
		_DeleteLocalRef(env,srClass);
		return NULL;
	}

	jstring result = base_cast(jstring,_CallObjectMethod(env,sr,toStringMID));
	if (result == NULL) {
		_DeleteLocalRef(env,srClass);
		return NULL;
	}

	_DeleteLocalRef(env,srClass);
	return result;
}

JNIEXPORTC char* JNICALL StringReaderToStringUTF(JNIEnv *env,jstringReader sr) {
	if (!env || !sr) return NULL;

	jstring jstr = StringWriterToString(env,sr);
	if (!jstr) return NULL;

	jsize utfLen = _GetStringUTFLength(env,jstr);
	const char *utf = _GetStringUTFChars(env,jstr,NULL);
	if (!utf) {
		_DeleteLocalRef(env,jstr);
		return NULL;
	}

	char *copy = ubase_cast(char*,malloc(base_cast(size_t,utfLen) + 1));
	if (!copy) {
		_ReleaseStringUTFChars(env,jstr,utf);
		_DeleteLocalRef(env,jstr);
		return NULL;
	}

	jmemcpy(copy,utf,base_cast(size_t,utfLen));
	copy[utfLen] = '\0';

	_ReleaseStringUTFChars(env,jstr,utf);
	_DeleteLocalRef(env,jstr);

	return copy;
}

JNIEXPORTC void JNICALL StringReaderEnsureOpen(JNIEnv *env,jstringReader sr) {
	jclass clz = _GetObjectClass(env,sr);
	jmethodID Id = _GetMethodID(env,clz,"ensureOpen","()V");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	_CallVoidMethod(env,sr,Id);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC jint JNICALL StringReaderRead(JNIEnv *env,jstringReader sr) {
	jclass clz = _GetObjectClass(env,sr);
	if (clz == NULL) return JNI_ERR;
	jmethodID Id = _GetMethodID(env,clz,"read","()I");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jint ret = _CallIntMethod(env,sr,Id);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jint JNICALL StringReaderReadCharArrayIntInt(JNIEnv *env,jstringReader sr,jcharArray obj,jint offset,jint len) {
	jclass clz = _GetObjectClass(env,sr);
	if (clz == NULL) return JNI_ERR;
	jmethodID Id = _GetMethodID(env,clz,"read","([CII)I");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jint ret = _CallIntMethod_l(env,sr,Id,obj,offset,len);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jlong JNICALL StringReaderReadSkip(JNIEnv *env,jstringReader sr,jlong ns) {
	jclass clz = _GetObjectClass(env,sr);
	if (clz == NULL) return JNI_ERR;
	jmethodID Id = _GetMethodID(env,clz,"skip","(J)J");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jlong ret = _CallLongMethod_l(env,sr,Id,ns);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jboolean JNICALL StringReaderReadReady(JNIEnv *env,jstringReader sr) {
	jclass clz = _GetObjectClass(env,sr);
	if (clz == NULL) return JNI_ERR;
	jmethodID Id = _GetMethodID(env,clz,"ready","()Z");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jboolean ret = _CallBooleanMethod(env,sr,Id);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jboolean JNICALL StringReaderReadMarkSupported(JNIEnv *env,jstringReader sr) {
	jclass clz = _GetObjectClass(env,sr);
	if (clz == NULL) return JNI_FALSE;
	jmethodID Id = _GetMethodID(env,clz,"markSupported","()Z");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jboolean ret = _CallBooleanMethod(env,sr,Id);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC void JNICALL StringReaderMark(JNIEnv *env,jstringReader sr,jint readAheadLimit) {
	jclass clz = _GetObjectClass(env,sr);
	if (clz == NULL) return;
	jmethodID Id = _GetMethodID(env,clz,"mark","(I)V");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	_CallVoidMethod_l(env,sr,Id,readAheadLimit);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringReaderReset(JNIEnv *env,jstringReader sr) {
	jclass clz = _GetObjectClass(env,sr);
	if (clz == NULL) return;
	jmethodID Id = _GetMethodID(env,clz,"reset","()V");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	_CallVoidMethod(env,sr,Id);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringReaderClose(JNIEnv *env,jstringReader sr) {
	jclass clz = _GetObjectClass(env,sr);
	if (clz == NULL) return;
	jmethodID Id = _GetMethodID(env,clz,"close","()V");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	_CallVoidMethod(env,sr,Id);
	_DeleteLocalRef(env,clz);
}
