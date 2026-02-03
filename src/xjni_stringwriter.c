#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <jni.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

// String Builder Utility
JNIEXPORTC jstringWriter JNICALL NewStringWriter(JNIEnv *env) {
	jclass clz = _FindClass(env,"java/io/StringWriter");
	if (!clz) {
		if (_ExceptionCheck(env)) {
			_ExceptionClear(env);
		}
		return NULL;
	}

	jmethodID ctor = _GetMethodID(env,clz,"<init>","()V");
	if (!ctor) {
		if (_ExceptionCheck(env)) {
			_ExceptionClear(env);
		}
		_DeleteLocalRef(env,clz);
		return NULL;
	}

	jobject obj = _NewObject(env,clz,ctor);

	if (_ExceptionCheck(env)) {
		_ExceptionClear(env);
		obj = NULL;
	}

	_DeleteLocalRef(env,clz);
	return obj;
}

JNIEXPORTC jstringWriter JNICALL NewStringWriterInitialSize(JNIEnv *env,jint initialSize) {
	jclass clz = _FindClass(env,"java/io/StringWriter");
	if (!clz) {
		if (_ExceptionCheck(env)) {
			_ExceptionClear(env);
		}
		return NULL;
	}

	jmethodID ctor = _GetMethodID(env,clz,"<init>","(I)V");
	if (!ctor) {
		if (_ExceptionCheck(env)) {
			_ExceptionClear(env);
		}
		_DeleteLocalRef(env,clz);
		return NULL;
	}

	jobject obj = _NewObject(env,clz,ctor,initialSize);

	if (_ExceptionCheck(env)) {
		_ExceptionClear(env);
		obj = NULL;
	}

	_DeleteLocalRef(env,clz);
	return obj;
}

JNIEXPORTC jstring JNICALL StringWriterToString(JNIEnv *env,jstringWriter sw) {
	if (env == NULL || sw == NULL) return NULL;

	jclass swClass = _GetObjectClass(env,sw);
	if (swClass == NULL) return NULL;

	jmethodID toStringMID = _GetMethodID(env,swClass,"toString","()Ljava/lang/String;");
	if (toStringMID == NULL) {
		_DeleteLocalRef(env,swClass);
		return NULL;
	}

	jstring result = base_cast(jstring,_CallObjectMethod(env,sw,toStringMID));
	if (result == NULL) {
		_DeleteLocalRef(env,swClass);
		return NULL;
	}

	_DeleteLocalRef(env,swClass);
	return result;
}

JNIEXPORTC char* JNICALL StringWriterToStringUTF(JNIEnv *env,jstringWriter sw) {
	if (!env || !sw) return NULL;

	jstring jstr = StringWriterToString(env,sw);
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

JNIEXPORTC void JNICALL StringWriterWriteCharArrayIntInt(JNIEnv *env,jstringWriter sw,jcharArray obj,jint offset,jint len) {
	jclass clz = _GetObjectClass(env,sw);
	if (clz == NULL) return;
	jmethodID Id = _GetMethodID(env,clz,"write","([CII)V");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	_CallVoidMethod(env,sw,Id,obj,offset,len);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringWriterWriteInt(JNIEnv *env,jstringWriter sw,jint len) {
	jclass clz = _GetObjectClass(env,sw);
	if (clz == NULL) return;
	jmethodID Id = _GetMethodID(env,clz,"write","(I)V");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	_CallVoidMethod(env,sw,Id,len);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringWriterWriteString(JNIEnv *env,jstringWriter sw,jstring str) {
	jclass clz = _GetObjectClass(env,sw);
	if (clz == NULL) return;
	jmethodID Id = _GetMethodID(env,clz,"write","(Ljava/lang/String;)V");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	_CallVoidMethod(env,sw,Id,str);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringWriterWriteStringIntInt(JNIEnv *env,jstringWriter sw,jstring str,jint offset,jint len) {
	jclass clz = _GetObjectClass(env,sw);
	if (clz == NULL) return;
	jmethodID Id = _GetMethodID(env,clz,"write","(Ljava/lang/String;II)V");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	_CallVoidMethod(env,sw,Id,str,offset,len);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringWriterFlush(JNIEnv *env,jstringWriter sw) {
	jclass clz = _GetObjectClass(env,sw);
	if (clz == NULL) return;
	jmethodID Id = _GetMethodID(env,clz,"flush","()V");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	_CallVoidMethod(env,sw,Id);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringWriterAppendChar(JNIEnv *env,jstringWriter sb,jchar obj) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID Id = _GetMethodID(env,clz,"append","(C)Ljava/io/StringWriter;");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod(env,sb,Id,obj);
	_DeleteLocalRef(env,clz);
}
