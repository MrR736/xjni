#include <xjni_stringarray.h>
#include <stdlib.h>
#include <string.h>
#include <jni.h>
#include <stdio.h>

JNIEXPORT jobjectArray JNICALL Java_TestStringArray_createStringArray(JNIEnv *env, jobject thiz, jobjectArray input) {\
	jsize len = GetStringArrayLength(env, input);
	if (len == 0) return NULL;

	char **newStrings = (char **)malloc(sizeof(char *) * len);
	if (newStrings == NULL) return NULL;

	for (jsize i = 0; i < len; i++) {
		// Get the individual string element
		jstring element = (jstring)(*env)->GetObjectArrayElement(env, input, i);
		const char *nativeString = (*env)->GetStringUTFChars(env, element, NULL);
		if (nativeString == NULL) continue;

		size_t newStrLen = strlen(nativeString) + 8;
		newStrings[i] = (char *)malloc(newStrLen);
		if (newStrings[i] == NULL) {
			free(newStrings);
			return NULL;
		}
		snprintf(newStrings[i], newStrLen, "%s_native", nativeString);
		(*env)->ReleaseStringUTFChars(env, element, nativeString);
	}

	// Create a new string array for the result
	jobjectArray result = (*env)->NewObjectArray(env, len, (*env)->FindClass(env, "java/lang/String"), NULL);
	if (result == NULL) {
		free(newStrings);
		return NULL;
	}

	for (jsize i = 0; i < len; i++) {
		jstring newString = (*env)->NewStringUTF(env, newStrings[i]);
		(*env)->SetObjectArrayElement(env, result, i, newString);
		free(newStrings[i]);
	}
	free(newStrings);
	return result;
}

JNIEXPORT void JNICALL Java_TestStringArray_printStringArray(JNIEnv *env, jobject thiz, jobjectArray arr) {
	jsize len = GetStringArrayLength(env, arr);
	if (len == 0) return;
	for (jsize i = 0; i < len; i++) {
		jstring element = (jstring)(*env)->GetObjectArrayElement(env, arr, i);
		const char *str = (*env)->GetStringUTFChars(env, element, NULL);
		if (str == NULL) continue;
		printf("[%d]: %s\n", i, str);
		(*env)->ReleaseStringUTFChars(env, element, str);
	}
}
