#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <jni.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include "xjni_stringarray.h"

/**
 * Gets the length of the UTF-8 encoded string of a specific jstring in the array.
 * This is just a wrapper around _GetArrayLength, assuming your custom wrapper handles edge cases.
 */
jsize GetStringUTFArrayLength(JNIEnv *env, jobjectArray array) {
    if (array == NULL) return 0;
    return _GetArrayLength(env, array);  // Returns the length of the array (number of elements)
}

/**
 * Creates a new jobjectArray from an array of const char* (UTF-8 strings).
 */
jobjectArray NewStringUTFArray(JNIEnv *env, const char **utf, jsize count) {
    if (utf == NULL)
        return NULL;

    jobjectArray stringArray = _NewObjectArray(env, count, _FindClass(env, "java/lang/String"), NULL);
    if (stringArray == NULL)
        return NULL;

    for (jsize i = 0; i < count; i++) {
        jstring jstr = NULL;
        jstr = _NewStringUTF(env, utf[i]);
        if (jstr == NULL) {
            BASE_LOGE("Failed to create jstring from UTF-8 string: %s\n", utf[i]);
            return NULL;
        }
        _SetObjectArrayElement(env, stringArray, i, jstr);
    }

    return stringArray;
}

/**
 * Converts each jstring in the jobjectArray to a C-style string.
 * Returns an array of const char* pointers.
 */
const char **GetStringUTFArrayChars(JNIEnv *env, jobjectArray array, jboolean *isCopy) {
    // Get the length of the array (number of elements)
    jsize arrayLength = GetStringUTFArrayLength(env, array);

    // Allocate memory for an array of pointers to char (each element will point to a C-style string)
    const char **strArray = (const char **)malloc(arrayLength * sizeof(const char *));
    if (strArray == NULL) {
        BASE_LOGE("Memory allocation failed for string array\n");
        return NULL;
    }

    // Convert each jstring to a UTF-8 encoded string
    for (jsize i = 0; i < arrayLength; i++) {
        jstring str = (jstring)(uintptr_t)_GetObjectArrayElement(env, array, i); // Get jstring from array
        strArray[i] = _GetStringUTFChars(env, str, isCopy);  // Convert to C-style string
    }

    return strArray;
}

/**
 * Releases the memory allocated for each string in the array.
 * The mode is typically set to 0, but can be used to specify different release behaviors.
 */
void ReleaseStringUTFArrayChars(JNIEnv *env, jobjectArray array, const char **elements, jint mode) {
    // Get the number of elements in the string array
    jsize arrayLength = GetStringUTFArrayLength(env, array);

    // Release each string from the array (use JNI to release the memory)
    for (jsize i = 0; i < arrayLength; i++) {
        jstring str = (jstring)(uintptr_t)_GetObjectArrayElement(env, array, i);  // Get the jstring
        _ReleaseStringUTFChars(env, str, elements[i]);  // Release the C-style string
    }

    // Free the memory allocated for the array of C-style string pointers
    free((void*)elements);
}

void GetStringUTFArrayRegion(JNIEnv *env, jobjectArray str, jsize start, jsize len, char **buf) {
    if (str == NULL || buf == NULL || start < 0 || len < 0)
        return;  // Return if any invalid parameter is passed

    // Get the length of the jobjectArray (sanity check)
    jsize arrayLength = GetStringUTFArrayLength(env, str);
    if (start >= arrayLength || start + len > arrayLength)
        return;  // Out of bounds check

    for (jsize i = 0; i < len; i++) {
        jstring jstr = (jstring)(uintptr_t)_GetObjectArrayElement(env, str, start + i);  // Get jstring from array
        if (jstr != NULL) {
            buf[i] = (char *)_GetStringUTFChars(env, jstr, NULL);  // Convert to UTF-8 string
        } else {
            buf[i] = NULL;
        }
    }
}

void SetStringUTFArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const char **buf) {
    if (array == NULL || buf == NULL || start < 0 || len < 0)
        return;

    jsize arrayLength = _GetArrayLength(env, array);
    if (start >= arrayLength || start + len > arrayLength)
        return;  // Out of bounds check

    for (jsize i = 0; i < len; i++) {
        jstring jstr = NULL;
        if (buf[i] != NULL) {
            jstr = _NewStringUTF(env, buf[i]);
            if (jstr == NULL) {
                BASE_LOGE("Failed to create jstring from UTF-8 string: %s\n", buf[i]);
                return;
            }
        }
        _SetObjectArrayElement(env, array, start + i, jstr);  // Set the jstring in the array
    }
}

jobjectArray NewStringArray(JNIEnv *env, const jchar **unicode, jsize len, jsize n) {
    if (unicode == NULL || len < 0)
        return NULL;  // Return NULL for invalid input

    jobjectArray stringArray = _NewObjectArray(env, n, _FindClass(env, "java/lang/String"), NULL);
    if (stringArray == NULL)
        return NULL;  // Allocation failed

    // Iterate over the provided `unicode` array and create `jstring` objects for each
    for (jsize i = 0; i < n; i++) {
        jstring jstr = _NewString(env, unicode[i], len);  // Create a jstring from jchar array
        if (jstr == NULL) {
            BASE_LOGE("Failed to create jstring from jchar array at index %d", i);
            return NULL;  // Return NULL on failure
        }
        _SetObjectArrayElement(env, stringArray, i, jstr);  // Set the jstring in the array
    }

    return stringArray;
}

jsize GetStringArrayLength(JNIEnv *env, jobjectArray array) {
    if (array == NULL) return 0;
    return _GetArrayLength(env, array);  // Returns the number of elements in the jobjectArray
}

const jchar **GetStringArrayChars(JNIEnv *env, jobjectArray str, jboolean *isCopy) {
    // Check for null input
    if (str == NULL) {
        BASE_LOGE("Input jobjectArray is NULL\n");
        return NULL;
    }

    // Get the length of the string array (i.e., the number of jstring elements)
    jsize arrayLength = _GetArrayLength(env, str);
    if (arrayLength == 0) {
        BASE_LOGE("Empty string array\n");
        return NULL;
    }

    // Allocate memory for the array of jchar pointers (const jchar**)
    const jchar **strArray = (const jchar **)malloc(arrayLength * sizeof(const jchar *));
    if (strArray == NULL) {
        BASE_LOGE("Memory allocation failed for jchar array\n");
        return NULL;
    }

    // For each jstring in the array, get its jchar array (characters)
    for (jsize i = 0; i < arrayLength; i++) {
        jstring jstr = (jstring)(uintptr_t)_GetObjectArrayElement(env, str, i);  // Get the jstring
        if (jstr != NULL)
            strArray[i] = _GetStringChars(env, jstr, isCopy);
        else
            strArray[i] = NULL;
    }

    return strArray;  // Return the array of jchar pointers
}

void ReleaseStringArrayChars(JNIEnv *env, jobjectArray str, const jchar **chars) {
    // Check for null input
    if (str == NULL || chars == NULL) {
        BASE_LOGE("Input parameters are NULL\n");
        return;
    }

    // Get the number of elements in the string array
    jsize arrayLength = _GetArrayLength(env, str);
    if (arrayLength == 0) {
        BASE_LOGE("Empty string array\n");
        return;
    }

    // For each jstring in the array, release the jchar data
    for (jsize i = 0; i < arrayLength; i++) {
        jstring jstr = (jstring)(uintptr_t)_GetObjectArrayElement(env, str, i);  // Get the jstring
        if (jstr != NULL)
            _ReleaseStringChars(env, jstr, chars[i]);  // Release the jchar data for this string
    }

    // Free the memory allocated for the array of jchar pointers
    free((void *)chars);
}

void GetStringArrayRegion(JNIEnv *env, jobjectArray str, jsize start, jsize len, jchar **buf) {
    if (str == NULL || buf == NULL || start < 0 || len < 0)
        return;  // Return if any invalid parameter is passed

    jsize arrayLength = _GetArrayLength(env, str);
    if (start >= arrayLength || start + len > arrayLength)
        return;  // Out of bounds check

    // Copy a subset of characters from the jobjectArray into the buffer
    for (jsize i = 0; i < len; i++) {
        jstring jstr = (jstring)(uintptr_t)_GetObjectArrayElement(env, str, start + i);  // Get jstring from array
        if (jstr != NULL) {
            const jchar *chars = _GetStringChars(env, jstr, NULL);  // Get jchar array from the jstring
            memcpy(buf[i], chars, _GetStringLength(env, jstr) * sizeof(jchar));  // Copy to buf
        }
    }
}

void SetStringArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jchar **buf) {
    if (array == NULL || buf == NULL || start < 0 || len < 0)
        return;  // Return if any invalid parameter is passed

    jsize arrayLength = _GetArrayLength(env, array);
    if (start >= arrayLength || start + len > arrayLength)
        return;  // Out of bounds check

    // Set the region of the jobjectArray from the provided buffer
    for (jsize i = 0; i < len; i++) {
        jstring jstr = NULL;
        if (buf[i] != NULL) {
            jstr = _NewString(env, buf[i], len);  // Create jstring from jchar buffer
            if (jstr == NULL) {
                BASE_LOGE("Failed to create jstring from jchar array at index %d", i);
                return;  // Return on failure
            }
        }
        _SetObjectArrayElement(env, array, start + i, jstr);  // Set the jstring in the array
    }
}
