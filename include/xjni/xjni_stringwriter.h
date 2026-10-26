/**
 * @file xjni_stringwriter.h
 * @brief Extern JNI StringWriter Utility
 *
 * Provides helper functions to create and manipulate Java StringWriter objects via JNI.
 *
 * @author MrR736
 * @date 2025
 * @copyright MIT
 */

#ifndef __XJNI_STRINGWRITER_H__
#define __XJNI_STRINGWRITER_H__

#include <jni.h>

#include "xjnidef.h"

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
XJNIAPI jstringWriter XJNICALL NewStringWriter(JNIEnv *env);

/**
 * Create a new StringWriter with an initial buffer size.
 * @param initialSize The initial buffer capacity.
 */
XJNIAPI jstringWriter XJNICALL NewStringWriterInitialSize(JNIEnv *env, jint initialSize);
//@}

/** @name Conversion */
//@{
/**
 * Convert a StringWriter to a Java String.
 */
XJNIAPI jstring XJNICALL StringWriterToString(JNIEnv *env, jstringWriter sw);

/**
 * Convert a StringWriter to a heap-allocated modified UTF-8 C string.
 * Caller must free() the returned pointer.
 */
XJNIAPI char* XJNICALL StringWriterToStringUTF(JNIEnv *env, jstringWriter sw);
//@}

/** @name Writing */
//@{
/**
 * Write a portion of a Java char array to the StringWriter.
 * @param obj Char array to write from.
 * @param offset Starting index.
 * @param len Number of characters to write.
 */
XJNIAPI void XJNICALL StringWriterWriteCharArrayIntInt(JNIEnv *env, jstringWriter sw, jcharArray obj, jint offset, jint len);

/**
 * Write a single character (int) to the StringWriter.
 */
XJNIAPI void XJNICALL StringWriterWriteInt(JNIEnv *env, jstringWriter sw, jint len);

/**
 * Write a full Java String to the StringWriter.
 */
XJNIAPI void XJNICALL StringWriterWriteString(JNIEnv *env, jstringWriter sw, jstring str);

/**
 * Write a portion of a Java String to the StringWriter.
 * @param str The Java string.
 * @param offset Starting index.
 * @param len Number of characters to write.
 */
XJNIAPI void XJNICALL StringWriterWriteStringIntInt(JNIEnv *env, jstringWriter sw, jstring str, jint offset, jint len);
//@}

/** @name Flush and Append */
//@{
/**
 * Flush the StringWriter (no-op for memory-based writer, provided for API completeness).
 */
XJNIAPI void XJNICALL StringWriterFlush(JNIEnv *env, jstringWriter sw);

/**
 * Append a single Java char to the StringWriter.
 */
XJNIAPI void XJNICALL StringWriterAppendChar(JNIEnv *env, jstringWriter sw, jchar obj);
//@}

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRINGWRITER_H__ */
