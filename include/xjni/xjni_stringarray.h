/**
 * @file xjni_stringarray.h
 * @brief Extern JNI String Array Utility
 *
 * Provides helper functions to create, access, and manipulate Java String[] arrays via JNI.
 *
 * @author MrR736
 * @date 2025
 * @copyright GPL-3
 */

#ifndef __XJNI_STRINGARRAY_H__
#define __XJNI_STRINGARRAY_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup JNI_StringArray Java String Array Utilities
 *  Functions to create, access, and manipulate Java String[] arrays from native C/C++.
 *  @{
 */

/** @name UTF-8 String Array Operations */
//@{
/**
 * @brief Create a new Java String[] from a UTF-8 C string array.
 * @param env Pointer to the JNI environment.
 * @param utf Array of UTF-8 C strings.
 * @param count Number of elements in the array.
 * @return Java String[] object.
 */
JNIEXPORT jobjectArray JNICALL NewStringUTFArray(JNIEnv *env, const char **utf, jsize count);

/**
 * @brief Get UTF-8 C string pointers from a Java String[] array.
 * @param env Pointer to the JNI environment.
 * @param array Java String[] object.
 * @param isCopy Optional output, indicates if a copy was made.
 * @return Array of UTF-8 C strings. Do not free() directly; release with ReleaseStringUTFArrayChars().
 */
JNIEXPORT const char** JNICALL GetStringUTFArrayChars(JNIEnv *env, jobjectArray array, jboolean *isCopy);

/**
 * @brief Release UTF-8 C string array obtained from GetStringUTFArrayChars.
 * @param env Pointer to the JNI environment.
 * @param array Java String[] object.
 * @param elements Array of UTF-8 C strings obtained previously.
 * @param mode Mode for releasing (0 = copy back and free, JNI_ABORT = free without copy back).
 */
JNIEXPORT void JNICALL ReleaseStringUTFArrayChars(JNIEnv *env, jobjectArray array, const char **elements, jint mode);

/**
 * @brief Copy a region of Java String[] into a C buffer of UTF-8 strings.
 * @param env Pointer to the JNI environment.
 * @param str Java String[] object.
 * @param start Starting index.
 * @param len Number of elements to copy.
 * @param buf Output buffer of C strings.
 */
JNIEXPORT void JNICALL GetStringUTFArrayRegion(JNIEnv *env, jobjectArray str, jsize start, jsize len, char **buf);

/**
 * @brief Set a region of a Java String[] from a C buffer of UTF-8 strings.
 * @param env Pointer to the JNI environment.
 * @param array Java String[] object.
 * @param start Starting index.
 * @param len Number of elements to set.
 * @param buf Buffer of C strings.
 */
JNIEXPORT void JNICALL SetStringUTFArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const char **buf);

/**
 * @brief Get the length of a Java String[] array.
 * @param env Pointer to the JNI environment.
 * @param str Java String[] object.
 * @return Number of elements in the array.
 */
JNIEXPORT jsize JNICALL GetStringUTFArrayLength(JNIEnv *env, jobjectArray str);
//@}

/** @name Unicode (jchar) String Array Operations */
//@{
/**
 * @brief Create a new Java String[] from an array of Unicode (jchar) strings.
 * @param env Pointer to the JNI environment.
 * @param unicode Array of Unicode strings (jchar arrays).
 * @param len Length of each string.
 * @param n Number of strings.
 * @return Java String[] object.
 */
JNIEXPORT jobjectArray JNICALL NewStringArray(JNIEnv *env, const jchar **unicode, jsize len, jsize n);

/**
 * @brief Get the length of a Java String[] array.
 * @param env Pointer to the JNI environment.
 * @param str Java String[] object.
 * @return Number of elements in the array.
 */
JNIEXPORT jsize JNICALL GetStringArrayLength(JNIEnv *env, jobjectArray str);

/**
 * @brief Get Unicode strings (jchar arrays) from a Java String[].
 * @param env Pointer to the JNI environment.
 * @param str Java String[] object.
 * @param isCopy Optional output, indicates if a copy was made.
 * @return Array of jchar pointers. Release with ReleaseStringArrayChars().
 */
JNIEXPORT const jchar** JNICALL GetStringArrayChars(JNIEnv *env, jobjectArray str, jboolean *isCopy);

/**
 * @brief Release Unicode strings obtained from GetStringArrayChars.
 * @param env Pointer to the JNI environment.
 * @param str Java String[] object.
 * @param chars Array of jchar pointers obtained previously.
 */
JNIEXPORT void JNICALL ReleaseStringArrayChars(JNIEnv *env, jobjectArray str, const jchar **chars);

/**
 * @brief Copy a region of Java String[] into a C buffer of jchar strings.
 * @param env Pointer to the JNI environment.
 * @param str Java String[] object.
 * @param start Starting index.
 * @param len Number of elements to copy.
 * @param buf Output buffer of jchar strings.
 */
JNIEXPORT void JNICALL GetStringArrayRegion(JNIEnv *env, jobjectArray str, jsize start, jsize len, jchar **buf);

/**
 * @brief Set a region of a Java String[] from a C buffer of jchar strings.
 * @param env Pointer to the JNI environment.
 * @param array Java String[] object.
 * @param start Starting index.
 * @param len Number of elements to set.
 * @param buf Buffer of jchar strings.
 */
JNIEXPORT void JNICALL SetStringArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jchar **buf);
//@}

/** @} */ // end of JNI_StringArray group

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRINGARRAY_H__ */
