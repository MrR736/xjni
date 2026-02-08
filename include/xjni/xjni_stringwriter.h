/**
 * @file xjni_stringwriter.h
 * @brief Extern JNI StringWriter Utility
 *
 * Provides helper functions to create and manipulate Java StringWriter objects via JNI.
 *
 * @author MrR736
 * @date 2025
 * @copyright GPL-3
 */

#ifndef __XJNI_STRINGWRITER_H__
#define __XJNI_STRINGWRITER_H__

#include <jni.h>

/** @typedef jstringWriter
 *  Represents a Java StringWriter object.
 */
typedef jobject jstringWriter;

#ifdef __cplusplus
extern "C" {
#endif

/** @name Constructors */
//@{
/**
 * Create a new StringWriter.
 */
JNIEXPORT jstringWriter JNICALL NewStringWriter(JNIEnv *env);

/**
 * Create a new StringWriter with an initial buffer size.
 * @param initialSize The initial buffer capacity.
 */
JNIEXPORT jstringWriter JNICALL NewStringWriterInitialSize(JNIEnv *env, jint initialSize);
//@}

/** @name Conversion */
//@{
/**
 * Convert a StringWriter to a Java String.
 */
JNIEXPORT jstring JNICALL StringWriterToString(JNIEnv *env, jstringWriter sw);

/**
 * Convert a StringWriter to a heap-allocated modified UTF-8 C string.
 * Caller must free() the returned pointer.
 */
JNIEXPORT char* JNICALL StringWriterToStringUTF(JNIEnv *env, jstringWriter sw);
//@}

/** @name Writing */
//@{
/**
 * Write a portion of a Java char array to the StringWriter.
 * @param obj Char array to write from.
 * @param offset Starting index.
 * @param len Number of characters to write.
 */
JNIEXPORT void JNICALL StringWriterWriteCharArrayIntInt(JNIEnv *env, jstringWriter sw, jcharArray obj, jint offset, jint len);

/**
 * Write a single character (int) to the StringWriter.
 */
JNIEXPORT void JNICALL StringWriterWriteInt(JNIEnv *env, jstringWriter sw, jint len);

/**
 * Write a full Java String to the StringWriter.
 */
JNIEXPORT void JNICALL StringWriterWriteString(JNIEnv *env, jstringWriter sw, jstring str);

/**
 * Write a portion of a Java String to the StringWriter.
 * @param str The Java string.
 * @param offset Starting index.
 * @param len Number of characters to write.
 */
JNIEXPORT void JNICALL StringWriterWriteStringIntInt(JNIEnv *env, jstringWriter sw, jstring str, jint offset, jint len);
//@}

/** @name Flush and Append */
//@{
/**
 * Flush the StringWriter (no-op for memory-based writer, provided for API completeness).
 */
JNIEXPORT void JNICALL StringWriterFlush(JNIEnv *env, jstringWriter sw);

/**
 * Append a single Java char to the StringWriter.
 */
JNIEXPORT void JNICALL StringWriterAppendChar(JNIEnv *env, jstringWriter sw, jchar obj);
//@}

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRINGWRITER_H__ */
