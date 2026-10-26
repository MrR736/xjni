#include <stdlib.h>

#include "base-jni.h"

#include <xjnidef.h>
#include <xjni_log.h>
#include <xjni_string.h>
#include <xjni_stringarray.h>

XJNIAPI jsize XJNICALL GetStringUTFArrayLength(JNIEnv *env,jobjectArray array) {
	if (array == NULL) return 0;
	return _GetArrayLength(env,array);
}

XJNIAPI jobjectArray XJNICALL NewStringUTFArray(JNIEnv *env,const char **utf,jsize count) {
	if (utf == NULL)
		return NULL;

	jclass strclass = _FindClass(env,"java/lang/String");
	jobjectArray stringArray = _NewObjectArray(env,count,strclass,NULL);
	_DeleteLocalRef(env,strclass);
	if (stringArray == NULL)
		return NULL;

	for (jsize i = 0; i < count; i++) {
		jstring jstr = NULL;
		jstr = _NewStringUTF(env,utf[i]);
		if (jstr == NULL) {
			XJNI_LOGE("NewStringUTFArray","Failed to create jstring from UTF-8 string: %s\n",utf[i]);
			return NULL;
		}
		_SetObjectArrayElement(env,stringArray,i,jstr);
	}

	return stringArray;
}

XJNIAPI const char** XJNICALL GetStringUTFArrayChars(JNIEnv *env,jobjectArray array,jboolean *isCopy) {
	if (array == NULL) return NULL;
	jsize arrayLength = _GetArrayLength(env,array);

	const char **strArray = ubase_cast(const char**,malloc(arrayLength * sizeof(const char *)));
	if (strArray == NULL) {
		XJNI_LOGE("GetStringUTFArrayChars","Memory allocation failed for string array\n");
		return NULL;
	}

	for (jsize i = 0; i < arrayLength; i++) {
		jstring str = ubase_cast(jstring,_GetObjectArrayElement(env,array,i));
		strArray[i] = _GetStringUTFChars(env,str,isCopy);
	}

	return strArray;
}

XJNIAPI void XJNICALL ReleaseStringUTFArrayChars(JNIEnv *env,jobjectArray array,const char **elements,jint mode) {
	if (array == NULL) return;
	jsize arrayLength = _GetArrayLength(env,array);

	for (jsize i = 0; i < arrayLength; i++) {
		jstring str = ubase_cast(jstring,_GetObjectArrayElement(env,array,i));
		_ReleaseStringUTFChars(env,str,elements[i]);
	}

	free((void*)elements);
}

XJNIAPI void XJNICALL GetStringUTFArrayRegion(JNIEnv *env,jobjectArray str,jsize start,jsize len,char **buf) {
	if (str == NULL || buf == NULL || start < 0 || len < 0)
		return;

	if (str == NULL) return;
	jsize arrayLength = _GetArrayLength(env,str);
	if (start >= arrayLength || start + len > arrayLength)
		return;

	for (jsize i = 0; i < len; i++) {
		jstring jstr = ubase_cast(jstring,_GetObjectArrayElement(env,str,start + i));
		if (jstr != NULL) {
			buf[i] = ubase_cast(char*,_GetStringUTFChars(env,jstr,NULL));
		} else {
			buf[i] = NULL;
		}
	}
}

XJNIAPI void XJNICALL SetStringUTFArrayRegion(JNIEnv *env,jobjectArray array,jsize start,jsize len,const char **buf) {
	if (array == NULL || buf == NULL || start < 0 || len < 0)
		return;

	if (array == NULL) return;
	jsize arrayLength = _GetArrayLength(env,array);
	if (start >= arrayLength || start + len > arrayLength)
		return;

	for (jsize i = 0; i < len; i++) {
		jstring jstr = NULL;
		if (buf[i] != NULL) {
			jstr = _NewStringUTF(env,buf[i]);
			if (jstr == NULL) {
				XJNI_LOGE("SetStringUTFArrayRegion","Failed to create jstring from UTF-8 string: %s\n",buf[i]);
				return;
			}
		}
		_SetObjectArrayElement(env,array,start + i,jstr);
	}
}

XJNIAPI jobjectArray XJNICALL NewStringArray(JNIEnv *env,const jchar **unicode,jsize len,jsize n) {
	if (unicode == NULL || len < 0)
		return NULL;

	jclass strclass = _FindClass(env,"java/lang/String");
	jobjectArray stringArray = _NewObjectArray(env,n,strclass,NULL);
	_DeleteLocalRef(env,strclass);
	if (stringArray == NULL)
		return NULL;


	for (jsize i = 0; i < n; i++) {
		jstring jstr = _NewString(env,unicode[i],len);
		if (jstr == NULL) {
			XJNI_LOGE("NewStringArray","Failed to create jstring from jchar array at index %d",i);
			return NULL;
		}
		_SetObjectArrayElement(env,stringArray,i,jstr);
	}

	return stringArray;
}

XJNIAPI jsize XJNICALL GetStringArrayLength(JNIEnv *env,jobjectArray array) {
	if (array == NULL) return 0;
	return _GetArrayLength(env,array);
}

XJNIAPI const jchar** XJNICALL GetStringArrayChars(JNIEnv *env,jobjectArray str,jboolean *isCopy) {
	if (str == NULL) {
		XJNI_LOGE("GetStringArrayChars","Input jobjectArray is NULL\n");
		return NULL;
	}

	if (str == NULL) return NULL;
	jsize arrayLength = _GetArrayLength(env,str);
	if (arrayLength == 0) {
		XJNI_LOGE("GetStringArrayChars","Empty string array\n");
		return NULL;
	}

	const jchar **strArray = ubase_cast(const jchar**,malloc(arrayLength * sizeof(const jchar*)));
	if (strArray == NULL) {
		XJNI_LOGE("","Memory allocation failed for jchar array\n");
		return NULL;
	}


	for (jsize i = 0; i < arrayLength; i++) {
		jstring jstr = ubase_cast(jstring,_GetObjectArrayElement(env,str,i));
		if (jstr != NULL) strArray[i] = _GetStringChars(env,jstr,isCopy);
		else strArray[i] = NULL;
	}

	return strArray;
}

XJNIAPI void XJNICALL ReleaseStringArrayChars(JNIEnv *env,jobjectArray str,const jchar **chars) {
	if (str == NULL || chars == NULL) {
		XJNI_LOGE("ReleaseStringArrayChars","Input parameters are NULL\n");
		return;
	}

	if (str == NULL) return;
	jsize arrayLength = _GetArrayLength(env,str);
	if (arrayLength == 0) {
		XJNI_LOGE("ReleaseStringArrayChars","Empty string array\n");
		return;
	}

	for (jsize i = 0; i < arrayLength; i++) {
		jstring jstr = ubase_cast(jstring,_GetObjectArrayElement(env,str,i));
		if (jstr != NULL)
			_ReleaseStringChars(env,jstr,chars[i]);
	}

	free((void*)chars);
}

XJNIAPI void XJNICALL GetStringArrayRegion(JNIEnv *env,jobjectArray str,jsize start,jsize len,jchar **buf) {
	if (str == NULL || buf == NULL || start < 0 || len < 0) return;
	if (str == NULL) return;
	jsize arrayLength = _GetArrayLength(env,str);
	if (start >= arrayLength || start + len > arrayLength) return;
	for (jsize i = 0; i < len; i++) {
		jstring jstr = ubase_cast(jstring,_GetObjectArrayElement(env,str,start + i));
		if (jstr != NULL) {
			const jchar *chars = _GetStringChars(env,jstr,NULL);
			jmemcpy(buf[i],chars,_GetStringLength(env,jstr) * sizeof(jchar));
		}
	}
}

XJNIAPI void XJNICALL SetStringArrayRegion(JNIEnv *env,jobjectArray array,jsize start,jsize len,const jchar **buf) {
	if (array == NULL || buf == NULL || start < 0 || len < 0) return;
	if (array == NULL) return;
	jsize arrayLength = _GetArrayLength(env,array);
	if (start >= arrayLength || start + len > arrayLength) return;
	for (jsize i = 0; i < len; i++) {
		jstring jstr = NULL;
		if (buf[i] != NULL) {
			jstr = _NewString(env,buf[i],len);
			if (jstr == NULL) {
				XJNI_LOGE("SetStringArrayRegion","Failed to create jstring from jchar array at index %d",i);
				return;
			}
		}
		_SetObjectArrayElement(env,array,start + i,jstr);
	}
}
