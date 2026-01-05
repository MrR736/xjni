#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <jni.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

#define MakeStringBufferAppend(name,type,sig)\
JNIEXPORTC void JNICALL name(JNIEnv *env,jstringBuffer sb,type obj) {\
	jclass clz = _GetObjectClass(env,sb);\
	if (clz == NULL) return;\
	jmethodID appendMethod = _GetMethodID(env,clz,"append",sig);\
	if (appendMethod == NULL) { _DeleteLocalRef(env,clz); return; }\
	sb = _CallObjectMethod_l(env,sb,appendMethod,obj);\
	_DeleteLocalRef(env,clz);\
}

#define MakeStringBufferInsert(name,type,sig)\
JNIEXPORTC void JNICALL name(JNIEnv *env,jstringBuffer sb,jint offset,type obj) {\
	jclass clz = _GetObjectClass(env,sb);\
	if (clz == NULL) return;\
	jmethodID appendMethod = _GetMethodID(env,clz,"insert",sig);\
	if (appendMethod == NULL) { _DeleteLocalRef(env,clz); return; }\
	sb = _CallObjectMethod_l(env,sb,appendMethod,offset,obj);\
	_DeleteLocalRef(env,clz);\
}

// String Builder Utility
JNIEXPORTC jstringBuffer JNICALL NewStringBuffer(JNIEnv *env) {
	jclass clz = _FindClass(env,"java/lang/StringBuffer");
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

JNIEXPORTC jstringBuffer JNICALL NewStringBufferCapacity(JNIEnv *env,jint capacity) {
	jclass clz = _FindClass(env,"java/lang/StringBuffer");
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

	jobject obj = _NewObject_l(env,clz,ctor,capacity);

	if (_ExceptionCheck(env)) {
		_ExceptionClear(env);
		obj = NULL;
	}

	_DeleteLocalRef(env,clz);
	return obj;
}

JNIEXPORT jobject JNICALL NewStringBufferString(JNIEnv *env,jstring str) {
	jclass clz = _FindClass(env,"java/lang/StringBuffer");
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

	jobject obj = _NewObject_l(env,clz,ctor,str);

	if (_ExceptionCheck(env)) {
		_ExceptionClear(env);
		obj = NULL;
	}

	_DeleteLocalRef(env,clz);
	return obj;
}

JNIEXPORTC jstringBuffer JNICALL NewStringBufferStringUTF(JNIEnv *env,const char* str) {
	if (str == NULL) return NULL;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return NULL;
	}
	jstringBuffer ret = NewStringBufferString(env,jstr);
	_DeleteLocalRef(env,jstr);
	return ret;
}

JNIEXPORTC jstring JNICALL StringBufferToString(JNIEnv *env,jstringBuffer sb) {
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

JNIEXPORTC char* JNICALL StringBufferToStringUTF(JNIEnv *env,jstringBuffer sb) {
	if (!env || !sb) return NULL;

	jstring jstr = StringBufferToString(env,sb);
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

MakeStringBufferAppend(StringBufferAppendString,jstring,"(Ljava/lang/String;)Ljava/lang/StringBuffer;")
MakeStringBufferAppend(StringBufferAppendObject,jobject,"(Ljava/lang/Object;)Ljava/lang/StringBuffer;")
MakeStringBufferAppend(StringBufferAppendStringBuffer,jstringBuffer,"(Ljava/lang/StringBuffer;)Ljava/lang/StringBuffer;")
MakeStringBufferAppend(StringBufferAppendBoolean,jboolean,"(Z)Ljava/lang/StringBuffer;")
MakeStringBufferAppend(StringBufferAppendCharArray,jcharArray,"([C)Ljava/lang/StringBuffer;")
MakeStringBufferAppend(StringBufferAppendChar,jchar,"(C)Ljava/lang/StringBuffer;")
MakeStringBufferAppend(StringBufferAppendInt,jint,"(I)Ljava/lang/StringBuffer;")
MakeStringBufferAppend(StringBufferAppendLong,jlong,"(J)Ljava/lang/StringBuffer;")
MakeStringBufferAppend(StringBufferAppendFloat,jfloat,"(F)Ljava/lang/StringBuffer;")
MakeStringBufferAppend(StringBufferAppendDouble,jdouble,"(D)Ljava/lang/StringBuffer;")

JNIEXPORTC void JNICALL StringBufferAppendStringUTF(JNIEnv *env,jstringBuffer sb,const char* str) {
	if (str == NULL) return;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	StringBufferAppendString(env,sb,jstr);
	_DeleteLocalRef(env,jstr);
}

JNIEXPORTC void JNICALL StringBufferAppendCharArrayIntInt(JNIEnv *env,jstringBuffer sb,jcharArray obj,jint offset,jint len) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID appendMethod = _GetMethodID(env,clz,"append","([CII)Ljava/lang/StringBuffer;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod_l(env,sb,appendMethod,obj,offset,len);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringBufferAppendCodePoint(JNIEnv *env,jstringBuffer sb,jint codePoint) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID appendMethod = _GetMethodID(env,clz,"appendCodePoint","(I)Ljava/lang/StringBuffer;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod_l(env,sb,appendMethod,codePoint);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringBufferDelete(JNIEnv *env,jstringBuffer sb,jint start,jint end) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID appendMethod = _GetMethodID(env,clz,"delete","(II)Ljava/lang/StringBuffer;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod_l(env,sb,appendMethod,start,end);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringBufferDeleteCharAt(JNIEnv *env,jstringBuffer sb,jint index) {
	jclass clz = _GetObjectClass(env,sb);
	jmethodID appendMethod = _GetMethodID(env,clz,"deleteCharAt","(I)Ljava/lang/StringBuffer;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod_l(env,sb,appendMethod,index);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringBufferReplace(JNIEnv *env,jstringBuffer sb,jint start,jint end,jstring str) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID appendMethod = _GetMethodID(env,clz,"delete","(IILjava/lang/String;)Ljava/lang/StringBuffer;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod_l(env,sb,appendMethod,start,end,str);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringBufferReplaceUTF(JNIEnv *env,jstringBuffer sb,jint start,jint end,const char* str) {
	if (str == NULL) return;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	StringBufferReplace(env,sb,start,end,jstr);
	_DeleteLocalRef(env,jstr);
}

JNIEXPORTC void JNICALL StringBufferInsertCharArrayIntInt(JNIEnv *env,jstringBuffer sb,jint index,jcharArray str,jint offset,jint len) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID appendMethod = _GetMethodID(env,clz,"insert","(I[CII)Ljava/lang/StringBuffer;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod_l(env,sb,appendMethod,index,str,offset,len);
	_DeleteLocalRef(env,clz);
}

MakeStringBufferInsert(StringBufferInsertString,jstring,"(ILjava/lang/String;)Ljava/lang/StringBuffer;")
MakeStringBufferInsert(StringBufferInsertObject,jobject,"(ILjava/lang/Object;)Ljava/lang/StringBuffer;")
MakeStringBufferInsert(StringBufferInsertCharArray,jcharArray,"(I[C)Ljava/lang/StringBuffer;")
MakeStringBufferInsert(StringBufferInsertBoolean,jboolean,"(IZ)Ljava/lang/StringBuffer;")
MakeStringBufferInsert(StringBufferInsertChar,jchar,"(IC)Ljava/lang/StringBuffer;")
MakeStringBufferInsert(StringBufferInsertInt,jint,"(II)Ljava/lang/StringBuffer;")
MakeStringBufferInsert(StringBufferInsertLong,jlong,"(IJ)Ljava/lang/StringBuffer;")
MakeStringBufferInsert(StringBufferInsertFloat,jfloat,"(IF)Ljava/lang/StringBuffer;")
MakeStringBufferInsert(StringBufferInsertDouble,jdouble,"(ID)Ljava/lang/StringBuffer;")

JNIEXPORTC void JNICALL StringBufferEnsureCapacity(JNIEnv *env,jstringBuffer sb,jint minimumCapacity) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID Id = _GetMethodID(env,clz,"ensureCapacity","(I)V");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	_CallVoidMethod_l(env,sb,Id,minimumCapacity);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringBufferSetLength(JNIEnv *env,jstringBuffer sb,jint newLength) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID Id = _GetMethodID(env,clz,"setLength","(I)V");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	_CallVoidMethod_l(env,sb,Id,newLength);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringBufferSetCharAt(JNIEnv *env,jstringBuffer sb,jint index,jchar ch) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID Id = _GetMethodID(env,clz,"setCharAt","(IC)V");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	_CallVoidMethod_l(env,sb,Id,index,ch);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC jchar JNICALL StringBufferCharAt(JNIEnv *env,jstringBuffer sb,jint index) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return '\0';
	jmethodID Id = _GetMethodID(env,clz,"charAt","(I)C");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return '\0';
	}
	jchar ret = _CallCharMethod_l(env,sb,Id,index);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jint JNICALL StringBufferCodePointAt(JNIEnv *env,jstringBuffer sb,jint index) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return JNI_ERR;
	jmethodID Id = _GetMethodID(env,clz,"codePointAt","(I)I");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jint ret = _CallIntMethod_l(env,sb,Id,index);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jint JNICALL StringBufferCodePointBefore(JNIEnv *env,jstringBuffer sb,jint index) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return JNI_ERR;
	jmethodID Id = _GetMethodID(env,clz,"codePointBefore","(I)I");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jint ret = _CallIntMethod_l(env,sb,Id,index);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jint JNICALL StringBufferCodePointCount(JNIEnv *env,jstringBuffer sb,jint beginIndex,jint endIndex) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return JNI_ERR;
	jmethodID Id = _GetMethodID(env,clz,"codePointCount","(II)I");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jint ret = _CallIntMethod_l(env,sb,Id,beginIndex,endIndex);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jint JNICALL StringBufferOffsetByCodePoints(JNIEnv *env,jstringBuffer sb,jint index,jint codePointOffset) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return JNI_ERR;
	jmethodID Id = _GetMethodID(env,clz,"offsetByCodePoints","(II)I");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jint ret = _CallIntMethod_l(env,sb,Id,index,codePointOffset);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC void JNICALL StringBufferTrimToSize(JNIEnv *env,jstringBuffer sb) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID Id = _GetMethodID(env,clz,"trimToSize","()V");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	_CallVoidMethod(env,sb,Id);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringBufferGetChars(JNIEnv *env,jstringBuffer sb,jint srcBegin,jint srcEnd,jcharArray dst,jint dstBegin) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID Id = _GetMethodID(env,clz,"getChars","(II[CI)V");
	if (Id == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	_CallVoidMethod_l(env,sb,Id,srcBegin,srcEnd,dst,dstBegin);
	_DeleteLocalRef(env,clz);
}

JNIEXPORTC void JNICALL StringBufferInsertStringUTF(JNIEnv *env,jstringBuffer sb,jint offset,const char* str) {
	if (str == NULL) return;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return;
	}
	StringBufferInsertString(env,sb,offset,jstr);
	_DeleteLocalRef(env,jstr);
}

JNIEXPORTC jstring JNICALL StringBufferSubString(JNIEnv *env,jstringBuffer sb,jint start) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return NULL;
	jmethodID appendMethod = _GetMethodID(env,clz,"substring","(I)Ljava/lang/String;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return NULL;
	}
	jstring ret = ubase_cast(jstring,_CallObjectMethod_l(env,sb,appendMethod,start));
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jstring JNICALL StringBufferSubStringIntInt(JNIEnv *env,jstringBuffer sb,jint start,jint end) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return NULL;
	jmethodID appendMethod = _GetMethodID(env,clz,"substring","(II)Ljava/lang/String;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return NULL;
	}
	jstring ret = ubase_cast(jstring,_CallObjectMethod_l(env,sb,appendMethod,start,end));
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jobject JNICALL StringBufferSubSequence(JNIEnv *env,jstringBuffer sb,jint start,jint end) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return NULL;
	jmethodID appendMethod = _GetMethodID(env,clz,"subSequence","(II)Ljava/lang/CharSequence;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return NULL;
	}
	jobject ret = _CallObjectMethod_l(env,sb,appendMethod,start,end);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jint JNICALL StringBufferIndexOf(JNIEnv *env,jstringBuffer sb,jstring str) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return JNI_ERR;
	jmethodID appendMethod = _GetMethodID(env,clz,"indexOf","(Ljava/lang/String;)I");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jint ret = _CallIntMethod_l(env,sb,appendMethod,str);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jint JNICALL StringBufferIndexOfI(JNIEnv *env,jstringBuffer sb,jstring str,jint fromIndex) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return JNI_ERR;
	jmethodID appendMethod = _GetMethodID(env,clz,"indexOf","(Ljava/lang/String;I)I");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jint ret = _CallIntMethod_l(env,sb,appendMethod,str,fromIndex);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jint JNICALL StringBufferIndexOfUTF(JNIEnv *env,jstringBuffer sb,const char* str) {
	if (str == NULL) return JNI_ERR;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return JNI_ERR;
	}
	jint ret = StringBufferIndexOf(env,sb,jstr);
	_DeleteLocalRef(env,jstr);
	return ret;
}

JNIEXPORTC jint JNICALL StringBufferIndexOfIUTF(JNIEnv *env,jstringBuffer sb,const char* str,jint fromIndex) {
	if (str == NULL) return JNI_ERR;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return JNI_ERR;
	}
	jint ret = StringBufferIndexOfI(env,sb,jstr,fromIndex);
	_DeleteLocalRef(env,jstr);
	return ret;
}

JNIEXPORTC jint JNICALL StringBufferLastIndexOf(JNIEnv *env,jstringBuffer sb,jstring str) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return JNI_ERR;
	jmethodID appendMethod = _GetMethodID(env,clz,"lastIndexOf","(Ljava/lang/String;)I");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jint ret = _CallIntMethod_l(env,sb,appendMethod,str);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jint JNICALL StringBufferLastIndexOfI(JNIEnv *env,jstringBuffer sb,jstring str,jint fromIndex) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return JNI_ERR;
	jmethodID appendMethod = _GetMethodID(env,clz,"lastIndexOf","(Ljava/lang/String;I)I");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jint ret = _CallIntMethod_l(env,sb,appendMethod,str,fromIndex);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jint JNICALL StringBufferLength(JNIEnv *env,jstringBuffer sb) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return JNI_ERR;
	jmethodID appendMethod = _GetMethodID(env,clz,"length","()I");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jint ret = _CallIntMethod(env,sb,appendMethod);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jint JNICALL StringBufferCapacity(JNIEnv *env,jstringBuffer sb) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return JNI_ERR;
	jmethodID appendMethod = _GetMethodID(env,clz,"capacity","()I");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return JNI_ERR;
	}
	jint ret = _CallIntMethod(env,sb,appendMethod);
	_DeleteLocalRef(env,clz);
	return ret;
}

JNIEXPORTC jint JNICALL StringBufferLastIndexOfUTF(JNIEnv *env,jstringBuffer sb,const char* str) {
	if (str == NULL) return JNI_ERR;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return JNI_ERR;
	}
	jint ret = StringBufferLastIndexOf(env,sb,jstr);
	_DeleteLocalRef(env,jstr);
	return ret;
}

JNIEXPORTC jint JNICALL StringBufferLastIndexOfIUTF(JNIEnv *env,jstringBuffer sb,const char* str,jint fromIndex) {
	if (str == NULL) return JNI_ERR;
	jstring jstr = _NewStringUTF(env,str);
	if (jstr == NULL || _ExceptionCheck(env)) {
		_ExceptionClear(env);
		return JNI_ERR;
	}
	jint ret = StringBufferLastIndexOfI(env,sb,jstr,fromIndex);
	_DeleteLocalRef(env,jstr);
	return ret;
}


JNIEXPORTC void JNICALL StringBufferReverse(JNIEnv *env,jstringBuffer sb) {
	jclass clz = _GetObjectClass(env,sb);
	if (clz == NULL) return;
	jmethodID appendMethod = _GetMethodID(env,clz,"reverse","()Ljava/lang/StringBuffer;");
	if (appendMethod == NULL) {
		_DeleteLocalRef(env,clz);
		return;
	}
	sb = _CallObjectMethod(env,sb,appendMethod);
	_DeleteLocalRef(env,clz);
}
