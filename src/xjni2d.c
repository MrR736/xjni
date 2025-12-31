#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <jni.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

#ifdef __cplusplus
#define _GetArrayElement(env,A,name,array,index) A name = (reinterpret_cast<A>(env->GetObjectArrayElement((array),(index))))
#define _GetArrayRegion(env,func,array,start,len,buf) env->func(array,start,len,buf)
#define _ReleaseArrayElements(env,func,array,elements,mode) env->func(array,elements,mode)
#define _GetArrayElements(env,func,array,elements) env->func(array,elements)
#else
#define _GetArrayElement(env,A,name,array,index) A name = ((A)(uintptr_t)((*env)->GetObjectArrayElement((env),(array),(index))))
#define _GetArrayRegion(env,func,array,start,len,buf) (*env)->func(env,array,start,len,buf)
#define _ReleaseArrayElements(env,func,array,elements,mode) (*env)->func(env,array,elements,mode)
#define _GetArrayElements(env,func,array,elements) (*env)->func(env,array,elements)
#endif

#define GetT2DArrayRegion(name,func,A, T) \
JNIEXPORTC void JNICALL name(JNIEnv *env, jobjectArray array, jsize start, jsize len, T **buf) {\
    if (buf == NULL) return; \
    jsize outer_len = _GetArrayLength(env, array); \
    if (start < 0 || len < 0 || start + len > outer_len) return; \
    for (jsize i = 0; i < len; ++i) { \
	T *dst = buf[i]; \
	if (dst == NULL) continue; \
	jsize index = start + i; \
	_GetArrayElement(env,A,inner,array,index); \
	if (inner != NULL) { \
	    jsize inner_len = _GetArrayLength(env,inner); \
	    _GetArrayRegion(env,func,inner,0,inner_len, dst); \
	    _DeleteLocalRef(env,inner);\
	} \
    } \
}

#define ReleaseT2DArrayElements(name,func,A, T) \
JNIEXPORTC void JNICALL name(JNIEnv *env, jobjectArray array, T **elements, jint mode) { \
    if (elements == NULL) return; \
    jsize outer_len = _GetArrayLength(env, array); \
    for (jsize i = 0; i < outer_len; ++i) { \
	if (elements[i] == NULL) continue; \
	_GetArrayElement(env,A,inner,array,i); \
	if (inner != NULL) { \
	    _ReleaseArrayElements(env,func,inner,elements[i],mode); \
	    _DeleteLocalRef(env,inner);\
	} \
    } \
    free((void*)elements); \
}

#define GetT2DArrayElements(name,func,A, T) \
JNIEXPORTC T** JNICALL name(JNIEnv *env, jobjectArray array, jboolean *isCopy) { \
    jsize outer_len = _GetArrayLength(env, array); \
    T **elements = ((T **)(calloc(outer_len, sizeof(T*)))); \
    if (!elements) return NULL; \
    jboolean anyCopy = JNI_FALSE; \
    for (jsize i = 0; i < outer_len; ++i) { \
	_GetArrayElement(env,A,inner,array,i); \
	if (inner == NULL) { \
	    elements[i] = NULL; \
	    continue; \
	} \
	jboolean localCopy = JNI_FALSE; \
	elements[i] = _GetArrayElements(env,func,inner,&localCopy); \
	_DeleteLocalRef(env,inner);\
	if (localCopy == JNI_TRUE) anyCopy = JNI_TRUE; \
    } \
    if (isCopy) *isCopy = anyCopy; \
    return elements; \
}

// Byte2D - Access and release functions for Java byte[][]
GetT2DArrayElements(GetByte2DArrayElements,GetByteArrayElements,jbyteArray,jbyte)
ReleaseT2DArrayElements(ReleaseByte2DArrayElements,ReleaseByteArrayElements,jbyteArray,jbyte)
GetT2DArrayRegion(SetByte2DArrayRegion,SetByteArrayRegion,jbyteArray,const jbyte)
GetT2DArrayRegion(GetByte2DArrayRegion,GetByteArrayRegion,jbyteArray,jbyte)

// Int2D - Access and release functions for Java int[][]
GetT2DArrayElements(GetInt2DArrayElements, GetIntArrayElements, jintArray, jint)
ReleaseT2DArrayElements(ReleaseInt2DArrayElements,ReleaseIntArrayElements,jintArray,jint)
GetT2DArrayRegion(SetInt2DArrayRegion,SetIntArrayRegion,jintArray,const jint)
GetT2DArrayRegion(GetInt2DArrayRegion,GetIntArrayRegion,jintArray,jint)

// Long2D - Access and release functions for Java long[][]
GetT2DArrayElements(GetLong2DArrayElements, GetLongArrayElements, jlongArray, jlong)
ReleaseT2DArrayElements(ReleaseLong2DArrayElements,ReleaseLongArrayElements,jlongArray,jlong)
GetT2DArrayRegion(SetLong2DArrayRegion,SetLongArrayRegion,jlongArray,const jlong)
GetT2DArrayRegion(GetLong2DArrayRegion,GetLongArrayRegion,jlongArray,jlong)

// Float2D - Access and release functions for Java float[][]
GetT2DArrayElements(GetFloat2DArrayElements, GetFloatArrayElements, jfloatArray, jfloat)
ReleaseT2DArrayElements(ReleaseFloat2DArrayElements,ReleaseFloatArrayElements,jfloatArray,jfloat)
GetT2DArrayRegion(SetFloat2DArrayRegion,SetFloatArrayRegion,jfloatArray,const jfloat)
GetT2DArrayRegion(GetFloat2DArrayRegion,GetFloatArrayRegion,jfloatArray,jfloat)

// Double2D - Access and release functions for Java double[][].
GetT2DArrayElements(GetDouble2DArrayElements, GetDoubleArrayElements, jdoubleArray, jdouble)
ReleaseT2DArrayElements(ReleaseDouble2DArrayElements,ReleaseDoubleArrayElements,jdoubleArray,jdouble)
GetT2DArrayRegion(SetDouble2DArrayRegion,SetDoubleArrayRegion,jdoubleArray, const jdouble)
GetT2DArrayRegion(GetDouble2DArrayRegion,GetDoubleArrayRegion,jdoubleArray, jdouble)

// Short2D - Access and release functions for Java short[][].
GetT2DArrayElements(GetShort2DArrayElements, GetShortArrayElements, jshortArray, jshort)
ReleaseT2DArrayElements(ReleaseShort2DArrayElements,ReleaseShortArrayElements,jshortArray,jshort)
GetT2DArrayRegion(SetShort2DArrayRegion,SetShortArrayRegion,jshortArray, const jshort)
GetT2DArrayRegion(GetShort2DArrayRegion,GetShortArrayRegion,jshortArray, jshort)

// Char2D - Access and release functions for Java char[][].
GetT2DArrayElements(GetChar2DArrayElements, GetCharArrayElements, jcharArray, jchar)
ReleaseT2DArrayElements(ReleaseChar2DArrayElements,ReleaseCharArrayElements,jcharArray,jchar)
GetT2DArrayRegion(SetChar2DArrayRegion,SetCharArrayRegion,jcharArray, const jchar)
GetT2DArrayRegion(GetChar2DArrayRegion,GetCharArrayRegion,jcharArray, jchar)

// StringUTF2D - Access and release functions for Java String[][].
JNIEXPORTC const char*** JNICALL GetStringUTF2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy) {
	jsize outer_len = _GetArrayLength(env, array);
	const char ***elements = ((const char ***)(uintptr_t)(calloc(outer_len, sizeof(const char**))));
	if (!elements) return NULL;
	jboolean anyCopy = JNI_FALSE;
	for (jsize i = 0; i < outer_len; ++i) {
		_GetArrayElement(env,jobjectArray,inner,array,i);
		if (inner == NULL) {
			elements[i] = NULL;
			continue;
		}
		jboolean localCopy = JNI_FALSE;
		elements[i] = GetStringUTFArrayChars(env,array,&localCopy);
		_DeleteLocalRef(env,inner);
		if (localCopy == JNI_TRUE) anyCopy = JNI_TRUE;
	}
	if (isCopy) *isCopy = anyCopy;
	return elements;
}

JNIEXPORTC void JNICALL ReleaseStringUTF2DArrayChars(JNIEnv *env, jobjectArray array, const char ***elements, jint mode) {
	if (elements == NULL) return;
	jsize outer_len = _GetArrayLength(env,array);
	for (jsize i = 0; i < outer_len; ++i) {
		if (elements[i] == NULL) continue;
		_GetArrayElement(env,jobjectArray,inner,array,i);
		if (inner != NULL) {
			ReleaseStringUTFArrayChars(env,inner,elements[i],mode);
			_DeleteLocalRef(env,inner);
		}
	}
	free((void*)elements);
}

JNIEXPORTC void JNICALL SetStringUTF2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const char ***buf) {
	if (buf == NULL) return;
	jsize outer_len = _GetArrayLength(env, array);
	if (start < 0 || len < 0 || start + len > outer_len) return;
	for (jsize i = 0; i < len; ++i) {
		const char **dst = buf[i];
		if (dst == NULL) continue;
		jsize index = start + i;
		_GetArrayElement(env,jobjectArray,inner,array,index);
		if (inner != NULL) {
			jsize inner_len = _GetArrayLength(env,inner);
			SetStringUTFArrayRegion(env,inner,0,inner_len,dst);
			_DeleteLocalRef(env,inner);
		}
	}
}

JNIEXPORTC void JNICALL GetStringUTF2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, char ***buf) {
	if (buf == NULL) return;
	jsize outer_len = _GetArrayLength(env, array);
	if (start < 0 || len < 0 || start + len > outer_len) return;
	for (jsize i = 0; i < len; ++i) {
		char **dst = buf[i];
		if (dst == NULL) continue;
		jsize index = start + i;
		_GetArrayElement(env,jobjectArray,inner,array,index);
		if (inner != NULL) {
			jsize inner_len = _GetArrayLength(env,inner);
			GetStringUTFArrayRegion(env,inner,0,inner_len,dst);
			_DeleteLocalRef(env,inner);
		}
	}
}

// String2D - Access and release functions for Java String[][].

JNIEXPORTC const jchar*** JNICALL GetString2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy) {
	jsize outer_len = _GetArrayLength(env, array);
	const jchar ***elements = ((const jchar ***)(uintptr_t)(calloc(outer_len, sizeof(const jchar**))));
	if (!elements) return NULL;
	jboolean anyCopy = JNI_FALSE;
	for (jsize i = 0; i < outer_len; ++i) {
		_GetArrayElement(env,jobjectArray,inner,array,i);
		if (inner == NULL) {
			elements[i] = NULL;
			continue;
		}
		jboolean localCopy = JNI_FALSE;
		elements[i] = GetStringArrayChars(env,array,&localCopy);
		_DeleteLocalRef(env,inner);
		if (localCopy == JNI_TRUE) anyCopy = JNI_TRUE;
	}
	if (isCopy) *isCopy = anyCopy;
	return elements;
}

JNIEXPORTC void JNICALL ReleaseString2DArrayChars(JNIEnv *env, jobjectArray array, const jchar ***elements) {
	if (elements == NULL) return;
	jsize outer_len = _GetArrayLength(env,array);
	for (jsize i = 0; i < outer_len; ++i) {
		if (elements[i] == NULL) continue;
		_GetArrayElement(env,jobjectArray,inner,array,i);
		if (inner != NULL) {
			ReleaseStringArrayChars(env,inner,elements[i]);
			_DeleteLocalRef(env,inner);
		}
	}
	free((void*)elements);
}

JNIEXPORTC void JNICALL SetString2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jchar ***buf) {
	if (buf == NULL) return;
	jsize outer_len = _GetArrayLength(env, array);
	if (start < 0 || len < 0 || start + len > outer_len) return;
	for (jsize i = 0; i < len; ++i) {
		const jchar **dst = buf[i];
		if (dst == NULL) continue;
		jsize index = start + i;
		_GetArrayElement(env,jobjectArray,inner,array,index);
		if (inner != NULL) {
			jsize inner_len = _GetArrayLength(env,inner);
			SetStringArrayRegion(env,inner,0,inner_len,dst);
			_DeleteLocalRef(env,inner);
		}
	}
}

JNIEXPORTC void JNICALL GetString2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jchar ***buf) {
	if (buf == NULL) return;
	jsize outer_len = _GetArrayLength(env, array);
	if (start < 0 || len < 0 || start + len > outer_len) return;
	for (jsize i = 0; i < len; ++i) {
		jchar **dst = buf[i];
		if (dst == NULL) continue;
		jsize index = start + i;
		_GetArrayElement(env,jobjectArray,inner,array,index);
		if (inner != NULL) {
			jsize inner_len = _GetArrayLength(env,inner);
			GetStringArrayRegion(env,inner,0,inner_len,dst);
			_DeleteLocalRef(env,inner);
		}
	}
}
