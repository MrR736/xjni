#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <jni.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

#define MakeStringBuilderAppend(name,type,sig)\
JNIEXPORTC void JNICALL name(JNIEnv *env,jstringBuilder sb,type obj) {\
	jclass clz = _GetObjectClass(env,sb);\
	if (clz == NULL) return;\
	jmethodID appendMethod = _GetMethodID(env,clz,"append",sig);\
	if (appendMethod == NULL) { _DeleteLocalRef(env,clz); return; }\
	sb = _CallObjectMethod(env,sb,appendMethod,obj);\
	_DeleteLocalRef(env,clz);\
}

#define MakeStringBuilderInsert(name,type,sig)\
JNIEXPORTC void JNICALL name(JNIEnv *env,jstringBuilder sb,jint offset,type obj) {\
	jclass clz = _GetObjectClass(env,sb);\
	if (clz == NULL) return;\
	jmethodID appendMethod = _GetMethodID(env,clz,"insert",sig);\
	if (appendMethod == NULL) { _DeleteLocalRef(env,clz); return; }\
	sb = _CallObjectMethod(env,sb,appendMethod,offset,obj);\
	_DeleteLocalRef(env,clz);\
}

// String Builder Utility
JNIEXPORTC jstringBuilder JNICALL NewStringBuilder(JNIEnv *env) {
	jclass clz = _FindClass(env,"java/lang/StringBuilder");
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

JNIEXPORTC jstringBuilder JNICALL NewStringBuilderCapacity(JNIEnv *env,jint capacity) {
	jclass clz = _FindClass(env,"java/lang/StringBuilder");
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

	jobject obj = _NewObject(env,clz,ctor,capacity);

	if (_ExceptionCheck(env)) {
		_ExceptionClear(env);
		obj = NULL;
	}

	_DeleteLocalRef(env,clz);
	return obj;
}

JNIEXPORTC jstringBuilder JNICALL NewStringBuilderString(JNIEnv *env,jstring str) {
	jclass clz = _FindClass(env,"java/lang/StringBuilder");
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

	jobject obj = _NewObject(env,clz,ctor,str);

	if (_ExceptionCheck(env)) {
		_ExceptionClear(env);
		obj = NULL;
	}

	_DeleteLocalRef(env,clz);
	return obj;
}

JNIEXPORTC jstringBuilder JNICALL NewStringBuilderStringUTF(JNIEnv *env,const char* str) {
	if (str == NULL) return NULL;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return NULL;
	}
	jstringBuilder ret = NewStringBuilderString(env,jstr);
	_DeleteLocalRef(env,jstr);
	return ret;
}

JNIEXPORTC jstring JNICALL StringBuilderToString(JNIEnv *env,jstringBuilder sb) {
	if (env == NULL || sb == NULL) return NULL;

	jclass sbClass = _GetObjectClass(env,sb);
	if (sbClass == NULL) return NULL;

	jmethodID toStringMID = _GetMethodID(env,sbClass,"toString","()Ljava/lang/String;");
	if (toStringMID == NULL) {
		_DeleteLocalRef(env,sbClass);
		return NULL;
	}

	jstring result = base_cast(jstring,_CallObjectMethod(env,sb,toStringMID));

	_DeleteLocalRef(env,sbClass);
	return result;
}

JNIEXPORTC char* JNICALL StringBuilderToStringUTF(JNIEnv *env,jstringBuilder sb) {
	if (!env || !sb) return NULL;

	jstring jstr = StringBuilderToString(env,sb);
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

MakeStringBuilderAppend(StringBuilderAppendString,jstring,"(Ljava/lang/String;)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendObject,jobject,"(Ljava/lang/Object;)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendStringBuffer,jstringBuffer,"(Ljava/lang/StringBuffer;)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendBoolean,jboolean,"(Z)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendChar,jchar,"(C)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendInt,jint,"(I)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendLong,jlong,"(J)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendFloat,jfloat,"(F)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendDouble,jdouble,"(D)Ljava/lang/StringBuilder;")

JNIEXPORTC void JNICALL StringBuilderAppendStringUTF(JNIEnv *env,jstringBuilder sb,const char* str) {
	if (str == NULL) return;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	StringBuilderAppendString(env,sb,jstr);
	_DeleteLocalRef(env,jstr);
}

JNIEXPORTC void JNICALL StringBuilderAppendCharArrayIntInt(JNIEnv *env,jstringBuilder sb,jcharArray obj,jint offset,jint len) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID appendMethod = _GetMethodID(env,clz,"append","([CII)Ljava/lang/StringBuilder;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod(env,sb,appendMethod,obj,offset,len);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringBuilderAppendCodePoint(JNIEnv *env,jstringBuilder sb,jint codePoint) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID appendMethod = _GetMethodID(env,clz,"appendCodePoint","(I)Ljava/lang/StringBuilder;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod(env,sb,appendMethod,codePoint);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringBuilderDelete(JNIEnv *env,jstringBuilder sb,jint start,jint end) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID appendMethod = _GetMethodID(env,clz,"delete","(II)Ljava/lang/StringBuilder;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod(env,sb,appendMethod,start,end);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringBuilderDeleteCharAt(JNIEnv *env,jstringBuilder sb,jint index) {
	jclass clz = _GetObjectClass(env,sb);
	jmethodID appendMethod = _GetMethodID(env,clz,"deleteCharAt","(I)Ljava/lang/StringBuilder;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod(env,sb,appendMethod,index);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringBuilderReplace(JNIEnv *env,jstringBuilder sb,jint start,jint end,jstring str) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID appendMethod = _GetMethodID(env,clz,"delete","(IILjava/lang/String;)Ljava/lang/StringBuilder;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod(env,sb,appendMethod,start,end,str);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringBuilderReplaceUTF(JNIEnv *env,jstringBuilder sb,jint start,jint end,const char* str) {
	if (str == NULL) return;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	StringBuilderReplace(env,sb,start,end,jstr);
	_DeleteLocalRef(env,jstr);
}

JNIEXPORTC void JNICALL StringBuilderInsertCharArrayIntInt(JNIEnv *env,jstringBuilder sb,jint index,jcharArray str,jint offset,jint len) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID appendMethod = _GetMethodID(env,clz,"insert","(I[CII)Ljava/lang/StringBuilder;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod(env,sb,appendMethod,index,str,offset,len);
	_DeleteLocalRef(env,clz);
}

MakeStringBuilderInsert(StringBuilderInsertString,jstring,"(ILjava/lang/String;)Ljava/lang/StringBuilder;")
MakeStringBuilderInsert(StringBuilderInsertObject,jobject,"(ILjava/lang/Object;)Ljava/lang/StringBuilder;")
MakeStringBuilderInsert(StringBuilderInsertCharArray,jcharArray,"(I[C)Ljava/lang/StringBuilder;")
MakeStringBuilderInsert(StringBuilderInsertBoolean,jboolean,"(IZ)Ljava/lang/StringBuilder;")
MakeStringBuilderInsert(StringBuilderInsertChar,jchar,"(IC)Ljava/lang/StringBuilder;")
MakeStringBuilderInsert(StringBuilderInsertInt,jint,"(II)Ljava/lang/StringBuilder;")
MakeStringBuilderInsert(StringBuilderInsertLong,jlong,"(IJ)Ljava/lang/StringBuilder;")
MakeStringBuilderInsert(StringBuilderInsertFloat,jfloat,"(IF)Ljava/lang/StringBuilder;")
MakeStringBuilderInsert(StringBuilderInsertDouble,jdouble,"(ID)Ljava/lang/StringBuilder;")

JNIEXPORTC void JNICALL StringBuilderInsertStringUTF(JNIEnv *env,jstringBuilder sb,jint offset,const char* str) {
	if (str == NULL) return;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	StringBuilderInsertString(env,sb,offset,jstr);
	_DeleteLocalRef(env,jstr);
}

JNIEXPORTC jint JNICALL StringBuilderIndexOf(JNIEnv *env,jstringBuilder sb,jstring str) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return JNI_ERR;
	jmethodID appendMethod = _GetMethodID(env,clz,"indexOf","(Ljava/lang/String;)I");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jint ret = _CallIntMethod(env,sb,appendMethod,str);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jint JNICALL StringBuilderIndexOfI(JNIEnv *env,jstringBuilder sb,jstring str,jint fromIndex) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return JNI_ERR;
	jmethodID appendMethod = _GetMethodID(env,clz,"indexOf","(Ljava/lang/String;I)I");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jint ret = _CallIntMethod(env,sb,appendMethod,str,fromIndex);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jint JNICALL StringBuilderIndexOfUTF(JNIEnv *env,jstringBuilder sb,const char* str) {
	if (str == NULL) return JNI_ERR;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return JNI_ERR;
	}
	jint ret = StringBuilderIndexOf(env,sb,jstr);
	_DeleteLocalRef(env,jstr);
	return ret;
}

JNIEXPORTC jint JNICALL StringBuilderIndexOfIUTF(JNIEnv *env,jstringBuilder sb,const char* str,jint fromIndex) {
	if (str == NULL) return JNI_ERR;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return JNI_ERR;
	}
	jint ret = StringBuilderIndexOfI(env,sb,jstr,fromIndex);
	_DeleteLocalRef(env,jstr);
	return ret;
}

JNIEXPORTC jint JNICALL StringBuilderLastIndexOf(JNIEnv *env,jstringBuilder sb,jstring str) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return JNI_ERR;
	jmethodID appendMethod = _GetMethodID(env,clz,"lastIndexOf","(Ljava/lang/String;)I");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	return _CallIntMethod(env,sb,appendMethod,str);
}

JNIEXPORTC jint JNICALL StringBuilderLastIndexOfI(JNIEnv *env,jstringBuilder sb,jstring str,jint fromIndex) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return JNI_ERR;
	jmethodID appendMethod = _GetMethodID(env,clz,"lastIndexOf","(Ljava/lang/String;I)I");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	return _CallIntMethod(env,sb,appendMethod,str,fromIndex);
}

JNIEXPORTC jint JNICALL StringBuilderLastIndexOfUTF(JNIEnv *env,jstringBuilder sb,const char* str) {
	if (str == NULL) return JNI_ERR;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return JNI_ERR;
	}
	jint ret = StringBuilderLastIndexOf(env,sb,jstr);
	_DeleteLocalRef(env,jstr);
	return ret;
}

JNIEXPORTC jint JNICALL StringBuilderLastIndexOfIUTF(JNIEnv *env,jstringBuilder sb,const char* str,jint fromIndex) {
	if (str == NULL) return JNI_ERR;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return JNI_ERR;
	}
	jint ret = StringBuilderLastIndexOfI(env,sb,jstr,fromIndex);
	_DeleteLocalRef(env,jstr);
	return ret;
}


JNIEXPORTC void JNICALL StringBuilderReverse(JNIEnv *env,jstringBuilder sb) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID appendMethod = _GetMethodID(env,clz,"reverse","()Ljava/lang/StringBuilder;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod(env,sb,appendMethod);
	_DeleteLocalRef(env,clz);
}
