/**
 * @file xjni_stringbuffer.h
 * @brief Extern JNI String Buffer Utility for C/C++.
 *
 * This header provides functions to create, manipulate, and query
 * Java StringBuffer objects from native C/C++ code using JNI.
 *
 * @author MrR736
 * @date 2025
 * @copyright MIT
 */


#ifndef __XJNI_STRINGBUFFER_H__
#define __XJNI_STRINGBUFFER_H__

#include <jni.h>
#include "xjnidef.h"

/** @typedef jstringBuffer
 *  Represents a Java StringBuffer object.
 */
typedef jobject jstringBuffer;

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup JNI_StringBuffer Java StringBuffer Utilities
 *  Functions to create and manipulate Java StringBuffer objects from native code.
 *  @{
 */

/**
 * @brief Creates a new empty Java StringBuffer.
 * @param env Pointer to the JNI environment.
 * @return A new Java StringBuffer object.
 */
XJNIAPI jstringBuffer XJNICALL NewStringBuffer(JNIEnv *env);

/**
 * @brief Creates a new Java StringBuffer with a specified initial capacity.
 * @param env Pointer to the JNI environment.
 * @param capacity Initial capacity of the StringBuffer.
 * @return A new Java StringBuffer object.
 */
XJNIAPI jstringBuffer XJNICALL NewStringBufferCapacity(JNIEnv *env, jint capacity);

/**
 * @brief Creates a new Java StringBuffer initialized with a given Java String.
 * @param env Pointer to the JNI environment.
 * @param str Java String used to initialize the StringBuffer.
 * @return A new Java StringBuffer object.
 */
XJNIAPI jstringBuffer XJNICALL NewStringBufferString(JNIEnv *env, jstring str);

/**
 * @brief Creates a new Java StringBuffer initialized with a UTF-8 C string.
 * @param env Pointer to the JNI environment.
 * @param str UTF-8 C string used to initialize the StringBuffer.
 * @return A new Java StringBuffer object.
 */
XJNIAPI jstringBuffer XJNICALL NewStringBufferStringUTF(JNIEnv *env, const char* str);

/**
 * @brief Converts a Java StringBuffer to a Java String.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object to convert.
 * @return Java String containing the contents of the StringBuffer.
 */
XJNIAPI jstring XJNICALL StringBufferToString(JNIEnv *env, jstringBuffer sb);

/**
 * @brief Converts a Java StringBuffer to a heap-allocated UTF-8 C string.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object to convert.
 * @return Heap-allocated UTF-8 string. Caller MUST free() the returned pointer.
 * @warning Caller is responsible for freeing the memory using free().
 */
XJNIAPI char* XJNICALL StringBufferToStringUTF(JNIEnv *env, jstringBuffer sb);

/**
 * @brief Returns a substring of a Java StringBuffer starting at a given index.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param start Starting index.
 * @return Java String containing the substring.
 */
XJNIAPI jstring XJNICALL StringBufferSubString(JNIEnv *env, jstringBuffer sb, jint start);

/**
 * @brief Returns a substring of a Java StringBuffer between two indices.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param start Starting index (inclusive).
 * @param end Ending index (exclusive).
 * @return Java String containing the substring.
 */
XJNIAPI jstring XJNICALL StringBufferSubStringIntInt(JNIEnv *env, jstringBuffer sb, jint start, jint end);

/**
 * @brief Returns a CharSequence of a Java StringBuffer between two indices.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param start Starting index (inclusive).
 * @param end Ending index (exclusive).
 * @return Java CharSequence object.
 */
XJNIAPI jobject XJNICALL StringBufferSubSequence(JNIEnv *env, jstringBuffer sb, jint start, jint end);

/**
 * @brief Appends a Java String to a Java StringBuffer.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param obj Java String to append.
 */
XJNIAPI void XJNICALL StringBufferAppendString(JNIEnv *env, jstringBuffer sb, jstring obj);

/**
 * @brief Appends a UTF-8 C string to a Java StringBuffer.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param str UTF-8 string to append.
 */
XJNIAPI void XJNICALL StringBufferAppendStringUTF(JNIEnv *env, jstringBuffer sb, const char* str);

/**
 * @brief Appends a Java Object to a Java StringBuffer.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param obj Java Object to append.
 */
XJNIAPI void XJNICALL StringBufferAppendObject(JNIEnv *env, jstringBuffer sb, jobject obj);

/**
 * @brief Appends another StringBuffer to this StringBuffer.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param obj StringBuffer object to append.
 */
XJNIAPI void XJNICALL StringBufferAppendStringBuffer(JNIEnv *env, jstringBuffer sb, jstringBuffer obj);

/**
 * @brief Appends primitive types (boolean, char, int, long, float, double) to a StringBuffer.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param obj Value to append (type-specific overloads).
 */
XJNIAPI void XJNICALL StringBufferAppendBoolean(JNIEnv *env, jstringBuffer sb, jboolean obj);
XJNIAPI void XJNICALL StringBufferAppendChar(JNIEnv *env, jstringBuffer sb, jchar obj);
XJNIAPI void XJNICALL StringBufferAppendCharArray(JNIEnv *env, jstringBuffer sb, jcharArray obj);
XJNIAPI void XJNICALL StringBufferAppendInt(JNIEnv *env, jstringBuffer sb, jint obj);
XJNIAPI void XJNICALL StringBufferAppendLong(JNIEnv *env, jstringBuffer sb, jlong obj);
XJNIAPI void XJNICALL StringBufferAppendFloat(JNIEnv *env, jstringBuffer sb, jfloat obj);
XJNIAPI void XJNICALL StringBufferAppendDouble(JNIEnv *env, jstringBuffer sb, jdouble obj);
XJNIAPI void XJNICALL StringBufferAppendCharArrayIntInt(JNIEnv *env, jstringBuffer sb, jcharArray obj, jint offset, jint len);
XJNIAPI void XJNICALL StringBufferAppendCodePoint(JNIEnv *env, jstringBuffer sb, jint codePoint);

/**
 * @brief Returns the length or capacity of the StringBuffer.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @return Length or capacity value.
 */
XJNIAPI jint XJNICALL StringBufferLength(JNIEnv *env, jstringBuffer sb);
XJNIAPI jint XJNICALL StringBufferCapacity(JNIEnv *env, jstringBuffer sb);

/**
 * @brief Modifies the capacity or length of the StringBuffer.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param minimumCapacity or newLength depending on function.
 */
XJNIAPI void XJNICALL StringBufferEnsureCapacity(JNIEnv *env, jstringBuffer sb, jint minimumCapacity);
XJNIAPI void XJNICALL StringBufferSetLength(JNIEnv *env, jstringBuffer sb, jint newLength);

/**
 * @brief Retrieves or modifies characters at specific positions.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param index Position index.
 * @param ch Character to set (for setter).
 */
XJNIAPI jchar XJNICALL StringBufferCharAt(JNIEnv *env, jstringBuffer sb, jint index);
XJNIAPI void XJNICALL StringBufferSetCharAt(JNIEnv *env, jstringBuffer sb, jint index, jchar ch);

/**
 * @brief Retrieves code points and performs code point operations.
 */
XJNIAPI jint XJNICALL StringBufferCodePointAt(JNIEnv *env, jstringBuffer sb, jint index);
XJNIAPI jint XJNICALL StringBufferCodePointBefore(JNIEnv *env, jstringBuffer sb, jint index);
XJNIAPI jint XJNICALL StringBufferCodePointCount(JNIEnv *env, jstringBuffer sb, jint beginIndex, jint endIndex);
XJNIAPI jint XJNICALL StringBufferOffsetByCodePoints(JNIEnv *env, jstringBuffer sb, jint index, jint codePointOffset);

/**
 * @brief Trims the capacity of the StringBuffer to match its current length.
 */
XJNIAPI void XJNICALL StringBufferTrimToSize(JNIEnv *env, jstringBuffer sb);

/**
 * @brief Copies a range of characters from the StringBuffer to a character array.
 */
XJNIAPI void XJNICALL StringBufferGetChars(JNIEnv *env, jstringBuffer sb, jint srcBegin, jint srcEnd, jcharArray dst, jint dstBegin);

/**
 * @brief Deletes or replaces a range of characters in the StringBuffer.
 */
XJNIAPI void XJNICALL StringBufferDelete(JNIEnv *env, jstringBuffer sb, jint start, jint end);
XJNIAPI void XJNICALL StringBufferDeleteCharAt(JNIEnv *env, jstringBuffer sb, jint index);
XJNIAPI void XJNICALL StringBufferReplace(JNIEnv *env, jstringBuffer sb, jint start, jint end, jstring str);
XJNIAPI void XJNICALL StringBufferReplaceUTF(JNIEnv *env, jstringBuffer sb, jint start, jint end, const char* str);

/**
 * @brief Inserts characters, strings, or objects into the StringBuffer at a specific position.
 */
XJNIAPI void XJNICALL StringBufferInsertCharArrayIntInt(JNIEnv *env, jstringBuffer sb, jint index, jcharArray str, jint offset, jint len);
XJNIAPI void XJNICALL StringBufferInsertString(JNIEnv *env, jstringBuffer sb, jint offset, jstring obj);
XJNIAPI void XJNICALL StringBufferInsertStringUTF(JNIEnv *env, jstringBuffer sb, jint offset, const char* str);
XJNIAPI void XJNICALL StringBufferInsertObject(JNIEnv *env, jstringBuffer sb, jint offset, jobject obj);
XJNIAPI void XJNICALL StringBufferInsertCharArray(JNIEnv *env, jstringBuffer sb, jint offset, jcharArray obj);
XJNIAPI void XJNICALL StringBufferInsertBoolean(JNIEnv *env, jstringBuffer sb, jint offset, jboolean obj);
XJNIAPI void XJNICALL StringBufferInsertChar(JNIEnv *env, jstringBuffer sb, jint offset, jchar obj);
XJNIAPI void XJNICALL StringBufferInsertInt(JNIEnv *env, jstringBuffer sb, jint offset, jint obj);
XJNIAPI void XJNICALL StringBufferInsertLong(JNIEnv *env, jstringBuffer sb, jint offset, jlong obj);
XJNIAPI void XJNICALL StringBufferInsertFloat(JNIEnv *env, jstringBuffer sb, jint offset, jfloat obj);
XJNIAPI void XJNICALL StringBufferInsertDouble(JNIEnv *env, jstringBuffer sb, jint offset, jdouble obj);

/**
 * @brief Finds the index of a substring or UTF-8 string in the StringBuffer.
 */
XJNIAPI jint XJNICALL StringBufferIndexOf(JNIEnv *env, jstringBuffer sb, jstring str);
XJNIAPI jint XJNICALL StringBufferIndexOfUTF(JNIEnv *env, jstringBuffer sb, const char* str);
XJNIAPI jint XJNICALL StringBufferIndexOfI(JNIEnv *env, jstringBuffer sb, jstring str, jint fromIndex);
XJNIAPI jint XJNICALL StringBufferIndexOfIUTF(JNIEnv *env, jstringBuffer sb, const char* str, jint fromIndex);

/**
 * @brief Finds the last index of a substring or UTF-8 string in the StringBuffer.
 */
XJNIAPI jint XJNICALL StringBufferLastIndexOf(JNIEnv *env, jstringBuffer sb, jstring str);
XJNIAPI jint XJNICALL StringBufferLastIndexOfUTF(JNIEnv *env, jstringBuffer sb, const char* str);
XJNIAPI jint XJNICALL StringBufferLastIndexOfI(JNIEnv *env, jstringBuffer sb, jstring str, jint fromIndex);
XJNIAPI jint XJNICALL StringBufferLastIndexOfIUTF(JNIEnv *env, jstringBuffer sb, const char* str, jint fromIndex);

/**
 * @brief Reverses the contents of the StringBuffer.
 */
XJNIAPI void XJNICALL StringBufferReverse(JNIEnv *env, jstringBuffer sb);

/** @} */ // end of JNI_StringBuffer group

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRINGBUFFER_H__ */
