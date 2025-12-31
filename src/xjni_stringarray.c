#include <stdlib.h>
#include <jni.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

JNIEXPORTC jsize JNICALL GetStringUTFArrayLength(JNIEnv *env,jobjectArray array) {
	if (array == NULL) return 0;
	return _GetArrayLength(env,array);
}

JNIEXPORTC jobjectArray JNICALL NewStringUTFArray(JNIEnv *env,const char **utf,jsize count) {
	if (utf == NULL)
		return NULL;

	jclass strclass = _FindClass(env,"java/lang/String");
	jobjectArray stringArray = _NewObjectArray(env,count,strclass,NULL);
	_DeleteGlobalRef(env, strclass);
	if (stringArray == NULL)
		return NULL;

	for (jsize i = 0; i < count; i++) {
		jstring jstr = NULL;
		jstr = _NewStringUTF(env,utf[i]);
		if (jstr == NULL) {
			BASE_LOGE("Failed to create jstring from UTF-8 string: %s\n",utf[i]);
			return NULL;
		}
		_SetObjectArrayElement(env,stringArray,i,jstr);
	}

	return stringArray;
}

JNIEXPORTC const char** JNICALL GetStringUTFArrayChars(JNIEnv *env,jobjectArray array,jboolean *isCopy) {
	if (array == NULL) return NULL;
	jsize arrayLength = _GetArrayLength(env,array);

	const char **strArray = ubase_cast(const char**,malloc(arrayLength * sizeof(const char *)));
	if (strArray == NULL) {
		BASE_LOGE("Memory allocation failed for string array\n");
		return NULL;
	}

	for (jsize i = 0; i < arrayLength; i++) {
		jstring str = ubase_cast(jstring,_GetObjectArrayElement(env,array,i));
		strArray[i] = _GetStringUTFChars(env,str,isCopy);
	}

	return strArray;
}

JNIEXPORTC void JNICALL ReleaseStringUTFArrayChars(JNIEnv *env,jobjectArray array,const char **elements,jint mode) {
	if (array == NULL) return;
	jsize arrayLength = _GetArrayLength(env,array);

	for (jsize i = 0; i < arrayLength; i++) {
		jstring str = ubase_cast(jstring,_GetObjectArrayElement(env,array,i));
		_ReleaseStringUTFChars(env,str,elements[i]);
	}

	free((void*)elements);
}

JNIEXPORTC void JNICALL GetStringUTFArrayRegion(JNIEnv *env,jobjectArray str,jsize start,jsize len,char **buf) {
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

JNIEXPORTC void JNICALL SetStringUTFArrayRegion(JNIEnv *env,jobjectArray array,jsize start,jsize len,const char **buf) {
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
				BASE_LOGE("Failed to create jstring from UTF-8 string: %s\n",buf[i]);
				return;
			}
		}
		_SetObjectArrayElement(env,array,start + i,jstr);
	}
}

JNIEXPORTC jobjectArray JNICALL NewStringArray(JNIEnv *env,const jchar **unicode,jsize len,jsize n) {
	if (unicode == NULL || len < 0)
		return NULL;

	jclass strclass = _FindClass(env,"java/lang/String");
	jobjectArray stringArray = _NewObjectArray(env,n,strclass,NULL);
	_DeleteGlobalRef(env, strclass);
	if (stringArray == NULL)
		return NULL;


	for (jsize i = 0; i < n; i++) {
		jstring jstr = _NewString(env,unicode[i],len);
		if (jstr == NULL) {
			BASE_LOGE("Failed to create jstring from jchar array at index %d",i);
			return NULL;
		}
		_SetObjectArrayElement(env,stringArray,i,jstr);
	}

	return stringArray;
}

JNIEXPORTC jsize JNICALL GetStringArrayLength(JNIEnv *env,jobjectArray array) {
	if (array == NULL) return 0;
	return _GetArrayLength(env,array);
}

JNIEXPORTC const jchar** JNICALL GetStringArrayChars(JNIEnv *env,jobjectArray str,jboolean *isCopy) {
	if (str == NULL) {
		BASE_LOGE("Input jobjectArray is NULL\n");
		return NULL;
	}

	if (str == NULL) return NULL;
	jsize arrayLength = _GetArrayLength(env,str);
	if (arrayLength == 0) {
		BASE_LOGE("Empty string array\n");
		return NULL;
	}

	const jchar **strArray = ubase_cast(const jchar**,malloc(arrayLength * sizeof(const jchar*)));
	if (strArray == NULL) {
		BASE_LOGE("Memory allocation failed for jchar array\n");
		return NULL;
	}


	for (jsize i = 0; i < arrayLength; i++) {
		jstring jstr = ubase_cast(jstring,_GetObjectArrayElement(env,str,i));
		if (jstr != NULL)
			strArray[i] = _GetStringChars(env,jstr,isCopy);
		else
			strArray[i] = NULL;
	}

	return strArray;
}

JNIEXPORTC void JNICALL ReleaseStringArrayChars(JNIEnv *env,jobjectArray str,const jchar **chars) {
	if (str == NULL || chars == NULL) {
		BASE_LOGE("Input parameters are NULL\n");
		return;
	}

	if (str == NULL) return;
	jsize arrayLength = _GetArrayLength(env,str);
	if (arrayLength == 0) {
		BASE_LOGE("Empty string array\n");
		return;
	}

	for (jsize i = 0; i < arrayLength; i++) {
		jstring jstr = ubase_cast(jstring,_GetObjectArrayElement(env,str,i));
		if (jstr != NULL)
			_ReleaseStringChars(env,jstr,chars[i]);
	}

	free((void*)chars);
}

JNIEXPORTC void JNICALL GetStringArrayRegion(JNIEnv *env,jobjectArray str,jsize start,jsize len,jchar **buf) {
	if (str == NULL || buf == NULL || start < 0 || len < 0)
		return;

	if (str == NULL) return;
	jsize arrayLength = _GetArrayLength(env,str);
	if (start >= arrayLength || start + len > arrayLength)
		return;


	for (jsize i = 0; i < len; i++) {
		jstring jstr = ubase_cast(jstring,_GetObjectArrayElement(env,str,start + i));
		if (jstr != NULL) {
			const jchar *chars = _GetStringChars(env,jstr,NULL);
			jmemcpy(buf[i],chars,_GetStringLength(env,jstr) * sizeof(jchar));
		}
	}
}

JNIEXPORTC void JNICALL SetStringArrayRegion(JNIEnv *env,jobjectArray array,jsize start,jsize len,const jchar **buf) {
	if (array == NULL || buf == NULL || start < 0 || len < 0)
		return;

	if (array == NULL) return;
	jsize arrayLength = _GetArrayLength(env,array);
	if (start >= arrayLength || start + len > arrayLength)
		return;

	for (jsize i = 0; i < len; i++) {
		jstring jstr = NULL;
		if (buf[i] != NULL) {
			jstr = _NewString(env,buf[i],len);
			if (jstr == NULL) {
				BASE_LOGE("Failed to create jstring from jchar array at index %d",i);
				return;
			}
		}
		_SetObjectArrayElement(env,array,start + i,jstr);
	}
}
