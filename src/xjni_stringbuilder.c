#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <jni.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

#define MakeStringBuilderAppend(name,type,sig)\
JNIEXPORTC void JNICALL name(JNIEnv *env, jobject sb, type obj) {\
	jclass stringBuilderClass = _GetObjectClass(env,sb);\
	jmethodID appendMethod = _GetMethodID(env,stringBuilderClass,"append",sig);\
	if (appendMethod == NULL) return;\
	sb = _CallObjectMethod_l(env,sb,appendMethod,obj);\
}

#define MakeStringBuilderInsert(name,type,sig)\
JNIEXPORTC void JNICALL name(JNIEnv *env, jobject sb,jint offset, type obj) {\
	jclass stringBuilderClass = _GetObjectClass(env,sb);\
	jmethodID appendMethod = _GetMethodID(env,stringBuilderClass,"insert",sig);\
	if (appendMethod == NULL) return;\
	sb = _CallObjectMethod_l(env,sb,appendMethod,offset,obj);\
}

// String Builder Utility
JNIEXPORTC jobject JNICALL NewStringBuilder(JNIEnv *env) {
	jclass stringBuilderClass = _FindClass(env,"java/lang/StringBuilder");
	if (stringBuilderClass == NULL) return NULL;
	jmethodID constructor = _GetMethodID(env,stringBuilderClass, "<init>", "()V");
	if (constructor == NULL) return NULL;
	jobject stringBuilderObject =_NewObject(env,stringBuilderClass, constructor);
	if (stringBuilderObject == NULL) return NULL;
	return stringBuilderObject;
}

JNIEXPORTC jobject JNICALL NewStringBuilderCapacity(JNIEnv *env,jint capacity) {
	jclass stringBuilderClass = _FindClass(env,"java/lang/StringBuilder");
	if (stringBuilderClass == NULL) return NULL;
	jmethodID constructor = _GetMethodID(env,stringBuilderClass, "<init>", "(I)V");
	if (constructor == NULL) return NULL;
	jobject stringBuilderObject = _NewObject_l(env,stringBuilderClass,constructor,capacity);
	if (stringBuilderObject == NULL) return NULL;
	return stringBuilderObject;
}

JNIEXPORTC jobject JNICALL NewStringBuilderString(JNIEnv *env,jstring str) {
	jclass stringBuilderClass = _FindClass(env,"java/lang/StringBuilder");
	if (stringBuilderClass == NULL) return NULL;
	jmethodID constructor = _GetMethodID(env,stringBuilderClass, "<init>", "(Ljava/lang/String;)V");
	if (constructor == NULL) return NULL;
	jobject stringBuilderObject = _NewObject_l(env,stringBuilderClass,constructor,str);
	if (stringBuilderObject == NULL) return NULL;
	return stringBuilderObject;
}

JNIEXPORTC jobject JNICALL NewStringBuilderStringUTF(JNIEnv *env,const char* str) {
	if (str == NULL) return NULL;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return NULL;
	}
	jobject ret = NewStringBuilderString(env,jstr);
	_DeleteLocalRef(env, jstr);
	return ret;
}

JNIEXPORTC jstring JNICALL StringBuilderToString(JNIEnv *env, jobject sb) {
	if (env == NULL || sb == NULL) return NULL;

	jclass sbClass = _GetObjectClass(env, sb);
	if (sbClass == NULL) return NULL;

	jmethodID toStringMID = _GetMethodID(env, sbClass, "toString", "()Ljava/lang/String;");
	if (toStringMID == NULL) {
		_DeleteLocalRef(env, sbClass);
		return NULL;
	}

	jstring result = base_cast(jstring,_CallObjectMethod(env, sb, toStringMID));

	_DeleteLocalRef(env, sbClass);
	return result;
}

JNIEXPORTC char* JNICALL StringBuilderToStringUTF(JNIEnv *env, jobject sb) {
	if (!env || !sb) return NULL;

	jstring jstr = StringBuilderToString(env, sb);
	if (!jstr) return NULL;

	jsize utfLen = _GetStringUTFLength(env, jstr);
	const char *utf = _GetStringUTFChars(env, jstr, NULL);
	if (!utf) {
		_DeleteLocalRef(env, jstr);
		return NULL;
	}

	char *copy = ubase_cast(char*,malloc(base_cast(size_t,utfLen) + 1));
	if (!copy) {
		_ReleaseStringUTFChars(env, jstr, utf);
		_DeleteLocalRef(env, jstr);
		return NULL;
	}

	jmemcpy(copy, utf, base_cast(size_t,utfLen));
	copy[utfLen] = '\0';

	_ReleaseStringUTFChars(env, jstr, utf);
	_DeleteLocalRef(env, jstr);

	return copy;
}

MakeStringBuilderAppend(StringBuilderAppendString,jstring,"(Ljava/lang/String;)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendObject,jobject,"(Ljava/lang/Object;)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendStringBuffer,jobject,"(Ljava/lang/StringBuffer;)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendBoolean,jboolean,"(Z)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendChar,jchar,"(C)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendInt,jint,"(I)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendLong,jlong,"(J)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendFloat,jfloat,"(F)Ljava/lang/StringBuilder;")
MakeStringBuilderAppend(StringBuilderAppendDouble,jdouble,"(D)Ljava/lang/StringBuilder;")

JNIEXPORTC void JNICALL StringBuilderAppendStringUTF(JNIEnv *env, jobject sb, const char* str) {
	if (str == NULL) return;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	StringBuilderAppendString(env,sb,jstr);
	_DeleteLocalRef(env, jstr);
}

JNIEXPORTC void JNICALL StringBuilderAppendCharIntInt(JNIEnv *env, jobject sb,jcharArray obj,jint offset,jint len) {
	jclass stringBuilderClass = _GetObjectClass(env,sb);
	jmethodID appendMethod = _GetMethodID(env,stringBuilderClass,"append","([CII)Ljava/lang/StringBuilder;");
	if (appendMethod == NULL) return;
	sb = _CallObjectMethod_l(env,sb,appendMethod,obj,offset,len);
}

JNIEXPORTC void JNICALL StringBuilderAppendCodePoint(JNIEnv *env, jobject sb,jint codePoint) {
	jclass stringBuilderClass = _GetObjectClass(env,sb);
	jmethodID appendMethod = _GetMethodID(env,stringBuilderClass,"appendCodePoint","(I)Ljava/lang/StringBuilder;");
	if (appendMethod == NULL) return;
	sb = _CallObjectMethod_l(env,sb,appendMethod,codePoint);
}

JNIEXPORTC void JNICALL StringBuilderDelete(JNIEnv *env, jobject sb,jint start,jint end) {
	jclass stringBuilderClass = _GetObjectClass(env,sb);
	jmethodID appendMethod = _GetMethodID(env,stringBuilderClass,"delete","(II)Ljava/lang/StringBuilder;");
	if (appendMethod == NULL) return;
	sb = _CallObjectMethod_l(env,sb,appendMethod,start,end);
}

JNIEXPORTC void JNICALL StringBuilderDeleteCharAt(JNIEnv *env, jobject sb,jint index) {
	jclass stringBuilderClass = _GetObjectClass(env,sb);
	jmethodID appendMethod = _GetMethodID(env,stringBuilderClass,"deleteCharAt","(I)Ljava/lang/StringBuilder;");
	if (appendMethod == NULL) return;
	sb = _CallObjectMethod_l(env,sb,appendMethod,index);
}

JNIEXPORTC void JNICALL StringBuilderReplace(JNIEnv *env, jobject sb,jint start,jint end,jstring str) {
	jclass stringBuilderClass = _GetObjectClass(env,sb);
	jmethodID appendMethod = _GetMethodID(env,stringBuilderClass,"delete","(IILjava/lang/String;)Ljava/lang/StringBuilder;");
	if (appendMethod == NULL) return;
	sb = _CallObjectMethod_l(env,sb,appendMethod,start,end,str);
}

JNIEXPORTC void JNICALL StringBuilderReplaceUTF(JNIEnv *env, jobject sb,jint start,jint end,const char* str) {
	if (str == NULL) return;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	StringBuilderReplace(env,sb,start,end,jstr);
	_DeleteLocalRef(env, jstr);
}

JNIEXPORTC void JNICALL StringBuilderInsert(JNIEnv *env, jobject sb,jint index,jcharArray str,jint offset,jint len) {
	jclass stringBuilderClass = _GetObjectClass(env,sb);
	jmethodID appendMethod = _GetMethodID(env,stringBuilderClass,"insert","(I[CII)Ljava/lang/StringBuilder;");
	if (appendMethod == NULL) return;
	sb = _CallObjectMethod_l(env,sb,appendMethod,index,str,offset,len);
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

JNIEXPORTC void JNICALL StringBuilderInsertStringUTF(JNIEnv *env, jobject sb, jint offset,const char* str) {
	if (str == NULL) return;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	StringBuilderInsertString(env,sb,offset,jstr);
	_DeleteLocalRef(env, jstr);
}

JNIEXPORTC jint JNICALL StringBuilderIndexOf(JNIEnv *env, jobject sb, jstring str) {
	jclass stringBuilderClass = _GetObjectClass(env, sb);
	jmethodID appendMethod = _GetMethodID(env, stringBuilderClass, "indexOf", "(Ljava/lang/String;)I");
	if (appendMethod == NULL) return JNI_ERR;
	return _CallIntMethod_l(env, sb, appendMethod, str);
}

JNIEXPORTC jint JNICALL StringBuilderIndexOfI(JNIEnv *env, jobject sb,jstring str, jint fromIndex) {
	jclass stringBuilderClass = _GetObjectClass(env,sb);
	jmethodID appendMethod = _GetMethodID(env,stringBuilderClass,"indexOf","(Ljava/lang/String;I)I");
	if (appendMethod == NULL) return JNI_ERR;
	return _CallIntMethod_l(env,sb,appendMethod,str,fromIndex);
}

JNIEXPORTC jint JNICALL StringBuilderIndexOfUTF(JNIEnv *env,jobject sb,const char* str) {
	if (str == NULL) return JNI_ERR;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return JNI_ERR;
	}
	jint ret = StringBuilderIndexOf(env,sb,jstr);
	_DeleteLocalRef(env, jstr);
	return ret;
}

JNIEXPORTC jint JNICALL StringBuilderIndexOfIUTF(JNIEnv *env,jobject sb,const char* str, jint fromIndex) {
	if (str == NULL) return JNI_ERR;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return JNI_ERR;
	}
	jint ret = StringBuilderIndexOfI(env,sb,jstr,fromIndex);
	_DeleteLocalRef(env, jstr);
	return ret;
}

JNIEXPORTC jint JNICALL StringBuilderLastIndexOf(JNIEnv *env, jobject sb,jstring str) {
	jclass stringBuilderClass = _GetObjectClass(env,sb);
	jmethodID appendMethod = _GetMethodID(env,stringBuilderClass,"lastIndexOf","(Ljava/lang/String;)I");
	if (appendMethod == NULL) return JNI_ERR;
	return _CallIntMethod_l(env,sb,appendMethod,str);
}

JNIEXPORTC jint JNICALL StringBuilderLastIndexOfI(JNIEnv *env, jobject sb,jstring str, jint fromIndex) {
	jclass stringBuilderClass = _GetObjectClass(env,sb);
	jmethodID appendMethod = _GetMethodID(env,stringBuilderClass,"lastIndexOf","(Ljava/lang/String;I)I");
	if (appendMethod == NULL) return JNI_ERR;
	return _CallIntMethod_l(env,sb,appendMethod,str,fromIndex);
}

JNIEXPORTC jint JNICALL StringBuilderLastIndexOfUTF(JNIEnv *env,jobject sb,const char* str) {
	if (str == NULL) return JNI_ERR;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return JNI_ERR;
	}
	jint ret = StringBuilderLastIndexOf(env,sb,jstr);
	_DeleteLocalRef(env, jstr);
	return ret;
}

JNIEXPORTC jint JNICALL StringBuilderLastIndexOfIUTF(JNIEnv *env,jobject sb,const char* str, jint fromIndex) {
	if (str == NULL) return JNI_ERR;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return JNI_ERR;
	}
	jint ret = StringBuilderLastIndexOfI(env,sb,jstr,fromIndex);
	_DeleteLocalRef(env, jstr);
	return ret;
}


JNIEXPORTC void JNICALL StringBuilderReverse(JNIEnv *env, jobject sb) {
	jclass stringBuilderClass = _GetObjectClass(env,sb);
	jmethodID appendMethod = _GetMethodID(env,stringBuilderClass,"reverse","()Ljava/lang/StringBuilder;");
	if (appendMethod == NULL) return;
	sb = _CallObjectMethod(env,sb,appendMethod);
}
