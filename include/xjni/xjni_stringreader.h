/**
 * @file xjni_stringreader.h
 * @brief Extern JNI StringReader Utility
 *
 * Provides helper functions to create and manipulate Java StringReader objects via JNI.
 *
 * @author MrR736
 * @date 2025
 * @copyright GPL-3
 */

#ifndef __XJNI_STRINGREADER_H__
#define __XJNI_STRINGREADER_H__

#include <jni.h>

/** @typedef jstringReader
 *  @brief Represents a Java StringReader object.
 */
typedef jobject jstringReader;

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup JNI_StringReader Java StringReader Utilities
 *  Functions to create, read, and manipulate Java StringReader objects from C/C++.
 *  @{
 */

/** @name Constructors */
//@{
/**
 * @brief Create a new StringReader from a Java String.
 * @param env Pointer to the JNI environment.
 * @param str Java String to read from.
 * @return A new Java StringReader object.
 */
JNIEXPORT jstringReader JNICALL NewStringReader(JNIEnv *env, jstring str);

/**
 * @brief Create a new StringReader from a UTF-8 C string.
 * @param env Pointer to the JNI environment.
 * @param str UTF-8 C string to read from.
 * @return A new Java StringReader object.
 */
JNIEXPORT jstringReader JNICALL NewStringReaderUTF(JNIEnv *env, const char* str);
//@}

/** @name Conversion */
//@{
/**
 * @brief Convert the content of a StringReader to a Java String.
 * @param env Pointer to the JNI environment.
 * @param sr StringReader object.
 * @return Java String containing the content of the StringReader.
 */
JNIEXPORT jstring JNICALL StringReaderToString(JNIEnv *env, jstringReader sr);

/**
 * @brief Convert the content of a StringReader to a heap-allocated modified UTF-8 C string.
 * @param env Pointer to the JNI environment.
 * @param sr StringReader object.
 * @return Heap-allocated UTF-8 string. Caller MUST free() the returned pointer.
 * @warning Caller is responsible for freeing the memory using free().
 */
JNIEXPORT char* JNICALL StringReaderToStringUTF(JNIEnv *env, jstringReader sr);
//@}

/** @name Stream Operations */
//@{
/**
 * @brief Ensure the StringReader is open.
 * @param env Pointer to the JNI environment.
 * @param sr StringReader object.
 * @note Throws a Java IOException if the reader is closed.
 */
JNIEXPORT void JNICALL StringReaderEnsureOpen(JNIEnv *env, jstringReader sr);

/**
 * @brief Read a single character from the StringReader.
 * @param env Pointer to the JNI environment.
 * @param sr StringReader object.
 * @return Character as jint, or -1 if end-of-stream.
 */
JNIEXPORT jint JNICALL StringReaderRead(JNIEnv *env, jstringReader sr);

/**
 * @brief Read multiple characters into a Java char array.
 * @param env Pointer to the JNI environment.
 * @param sr StringReader object.
 * @param obj Destination Java char array.
 * @param offset Starting index in the array to store characters.
 * @param len Maximum number of characters to read.
 * @return Number of characters actually read, or -1 if end-of-stream.
 */
JNIEXPORT jint JNICALL StringReaderReadCharArrayIntInt(JNIEnv *env, jstringReader sr, jcharArray obj, jint offset, jint len);

/**
 * @brief Skip a specified number of characters in the stream.
 * @param env Pointer to the JNI environment.
 * @param sr StringReader object.
 * @param ns Number of characters to skip.
 * @return Actual number of characters skipped.
 */
JNIEXPORT jlong JNICALL StringReaderReadSkip(JNIEnv *env, jstringReader sr, jlong ns);

/**
 * @brief Check if the reader is ready to be read without blocking.
 * @param env Pointer to the JNI environment.
 * @param sr StringReader object.
 * @return JNI_TRUE if ready, JNI_FALSE otherwise.
 */
JNIEXPORT jboolean JNICALL StringReaderReadReady(JNIEnv *env, jstringReader sr);

/**
 * @brief Check if mark/reset operations are supported by this reader.
 * @param env Pointer to the JNI environment.
 * @param sr StringReader object.
 * @return JNI_TRUE if mark/reset is supported, JNI_FALSE otherwise.
 */
JNIEXPORT jboolean JNICALL StringReaderReadMarkSupported(JNIEnv *env, jstringReader sr);

/**
 * @brief Mark the current position in the stream.
 * @param env Pointer to the JNI environment.
 * @param sr StringReader object.
 * @param readAheadLimit Limit of characters that can be read before mark becomes invalid.
 */
JNIEXPORT void JNICALL StringReaderMark(JNIEnv *env, jstringReader sr, jint readAheadLimit);

/**
 * @brief Reset the stream to the last marked position.
 * @param env Pointer to the JNI environment.
 * @param sr StringReader object.
 */
JNIEXPORT void JNICALL StringReaderReset(JNIEnv *env, jstringReader sr);

/**
 * @brief Close the StringReader.
 * @param env Pointer to the JNI environment.
 * @param sr StringReader object.
 */
JNIEXPORT void JNICALL StringReaderClose(JNIEnv *env, jstringReader sr);
//@}

/** @} */ // end of JNI_StringReader group

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRINGREADER_H__ */
