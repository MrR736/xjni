/**
 * @file xjni_stringbuffer.h
 * @brief Extern JNI String Buffer Utility for C/C++.
 *
 * This header provides functions to create, manipulate, and query
 * Java StringBuffer objects from native C/C++ code using JNI.
 *
 * @author MrR736
 * @date 2025
 * @copyright GPL-3
 */


#ifndef __XJNI_STRINGBUFFER_H__
#define __XJNI_STRINGBUFFER_H__

#include <jni.h>

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
JNIEXPORT jstringBuffer JNICALL NewStringBuffer(JNIEnv *env);

/**
 * @brief Creates a new Java StringBuffer with a specified initial capacity.
 * @param env Pointer to the JNI environment.
 * @param capacity Initial capacity of the StringBuffer.
 * @return A new Java StringBuffer object.
 */
JNIEXPORT jstringBuffer JNICALL NewStringBufferCapacity(JNIEnv *env, jint capacity);

/**
 * @brief Creates a new Java StringBuffer initialized with a given Java String.
 * @param env Pointer to the JNI environment.
 * @param str Java String used to initialize the StringBuffer.
 * @return A new Java StringBuffer object.
 */
JNIEXPORT jstringBuffer JNICALL NewStringBufferString(JNIEnv *env, jstring str);

/**
 * @brief Creates a new Java StringBuffer initialized with a UTF-8 C string.
 * @param env Pointer to the JNI environment.
 * @param str UTF-8 C string used to initialize the StringBuffer.
 * @return A new Java StringBuffer object.
 */
JNIEXPORT jstringBuffer JNICALL NewStringBufferStringUTF(JNIEnv *env, const char* str);

/**
 * @brief Converts a Java StringBuffer to a Java String.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object to convert.
 * @return Java String containing the contents of the StringBuffer.
 */
JNIEXPORT jstring JNICALL StringBufferToString(JNIEnv *env, jstringBuffer sb);

/**
 * @brief Converts a Java StringBuffer to a heap-allocated UTF-8 C string.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object to convert.
 * @return Heap-allocated UTF-8 string. Caller MUST free() the returned pointer.
 * @warning Caller is responsible for freeing the memory using free().
 */
JNIEXPORT char* JNICALL StringBufferToStringUTF(JNIEnv *env, jstringBuffer sb);

/**
 * @brief Returns a substring of a Java StringBuffer starting at a given index.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param start Starting index.
 * @return Java String containing the substring.
 */
JNIEXPORT jstring JNICALL StringBufferSubString(JNIEnv *env, jstringBuffer sb, jint start);

/**
 * @brief Returns a substring of a Java StringBuffer between two indices.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param start Starting index (inclusive).
 * @param end Ending index (exclusive).
 * @return Java String containing the substring.
 */
JNIEXPORT jstring JNICALL StringBufferSubStringIntInt(JNIEnv *env, jstringBuffer sb, jint start, jint end);

/**
 * @brief Returns a CharSequence of a Java StringBuffer between two indices.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param start Starting index (inclusive).
 * @param end Ending index (exclusive).
 * @return Java CharSequence object.
 */
JNIEXPORT jobject JNICALL StringBufferSubSequence(JNIEnv *env, jstringBuffer sb, jint start, jint end);

/**
 * @brief Appends a Java String to a Java StringBuffer.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param obj Java String to append.
 */
JNIEXPORT void JNICALL StringBufferAppendString(JNIEnv *env, jstringBuffer sb, jstring obj);

/**
 * @brief Appends a UTF-8 C string to a Java StringBuffer.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param str UTF-8 string to append.
 */
JNIEXPORT void JNICALL StringBufferAppendStringUTF(JNIEnv *env, jstringBuffer sb, const char* str);

/**
 * @brief Appends a Java Object to a Java StringBuffer.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param obj Java Object to append.
 */
JNIEXPORT void JNICALL StringBufferAppendObject(JNIEnv *env, jstringBuffer sb, jobject obj);

/**
 * @brief Appends another StringBuffer to this StringBuffer.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param obj StringBuffer object to append.
 */
JNIEXPORT void JNICALL StringBufferAppendStringBuffer(JNIEnv *env, jstringBuffer sb, jstringBuffer obj);

/**
 * @brief Appends primitive types (boolean, char, int, long, float, double) to a StringBuffer.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param obj Value to append (type-specific overloads).
 */
JNIEXPORT void JNICALL StringBufferAppendBoolean(JNIEnv *env, jstringBuffer sb, jboolean obj);
JNIEXPORT void JNICALL StringBufferAppendChar(JNIEnv *env, jstringBuffer sb, jchar obj);
JNIEXPORT void JNICALL StringBufferAppendCharArray(JNIEnv *env, jstringBuffer sb, jcharArray obj);
JNIEXPORT void JNICALL StringBufferAppendInt(JNIEnv *env, jstringBuffer sb, jint obj);
JNIEXPORT void JNICALL StringBufferAppendLong(JNIEnv *env, jstringBuffer sb, jlong obj);
JNIEXPORT void JNICALL StringBufferAppendFloat(JNIEnv *env, jstringBuffer sb, jfloat obj);
JNIEXPORT void JNICALL StringBufferAppendDouble(JNIEnv *env, jstringBuffer sb, jdouble obj);
JNIEXPORT void JNICALL StringBufferAppendCharArrayIntInt(JNIEnv *env, jstringBuffer sb, jcharArray obj, jint offset, jint len);
JNIEXPORT void JNICALL StringBufferAppendCodePoint(JNIEnv *env, jstringBuffer sb, jint codePoint);

/**
 * @brief Returns the length or capacity of the StringBuffer.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @return Length or capacity value.
 */
JNIEXPORT jint JNICALL StringBufferLength(JNIEnv *env, jstringBuffer sb);
JNIEXPORT jint JNICALL StringBufferCapacity(JNIEnv *env, jstringBuffer sb);

/**
 * @brief Modifies the capacity or length of the StringBuffer.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param minimumCapacity or newLength depending on function.
 */
JNIEXPORT void JNICALL StringBufferEnsureCapacity(JNIEnv *env, jstringBuffer sb, jint minimumCapacity);
JNIEXPORT void JNICALL StringBufferSetLength(JNIEnv *env, jstringBuffer sb, jint newLength);

/**
 * @brief Retrieves or modifies characters at specific positions.
 * @param env Pointer to the JNI environment.
 * @param sb StringBuffer object.
 * @param index Position index.
 * @param ch Character to set (for setter).
 */
JNIEXPORT jchar JNICALL StringBufferCharAt(JNIEnv *env, jstringBuffer sb, jint index);
JNIEXPORT void JNICALL StringBufferSetCharAt(JNIEnv *env, jstringBuffer sb, jint index, jchar ch);

/**
 * @brief Retrieves code points and performs code point operations.
 */
JNIEXPORT jint JNICALL StringBufferCodePointAt(JNIEnv *env, jstringBuffer sb, jint index);
JNIEXPORT jint JNICALL StringBufferCodePointBefore(JNIEnv *env, jstringBuffer sb, jint index);
JNIEXPORT jint JNICALL StringBufferCodePointCount(JNIEnv *env, jstringBuffer sb, jint beginIndex, jint endIndex);
JNIEXPORT jint JNICALL StringBufferOffsetByCodePoints(JNIEnv *env, jstringBuffer sb, jint index, jint codePointOffset);

/**
 * @brief Trims the capacity of the StringBuffer to match its current length.
 */
JNIEXPORT void JNICALL StringBufferTrimToSize(JNIEnv *env, jstringBuffer sb);

/**
 * @brief Copies a range of characters from the StringBuffer to a character array.
 */
JNIEXPORT void JNICALL StringBufferGetChars(JNIEnv *env, jstringBuffer sb, jint srcBegin, jint srcEnd, jcharArray dst, jint dstBegin);

/**
 * @brief Deletes or replaces a range of characters in the StringBuffer.
 */
JNIEXPORT void JNICALL StringBufferDelete(JNIEnv *env, jstringBuffer sb, jint start, jint end);
JNIEXPORT void JNICALL StringBufferDeleteCharAt(JNIEnv *env, jstringBuffer sb, jint index);
JNIEXPORT void JNICALL StringBufferReplace(JNIEnv *env, jstringBuffer sb, jint start, jint end, jstring str);
JNIEXPORT void JNICALL StringBufferReplaceUTF(JNIEnv *env, jstringBuffer sb, jint start, jint end, const char* str);

/**
 * @brief Inserts characters, strings, or objects into the StringBuffer at a specific position.
 */
JNIEXPORT void JNICALL StringBufferInsertCharArrayIntInt(JNIEnv *env, jstringBuffer sb, jint index, jcharArray str, jint offset, jint len);
JNIEXPORT void JNICALL StringBufferInsertString(JNIEnv *env, jstringBuffer sb, jint offset, jstring obj);
JNIEXPORT void JNICALL StringBufferInsertStringUTF(JNIEnv *env, jstringBuffer sb, jint offset, const char* str);
JNIEXPORT void JNICALL StringBufferInsertObject(JNIEnv *env, jstringBuffer sb, jint offset, jobject obj);
JNIEXPORT void JNICALL StringBufferInsertCharArray(JNIEnv *env, jstringBuffer sb, jint offset, jcharArray obj);
JNIEXPORT void JNICALL StringBufferInsertBoolean(JNIEnv *env, jstringBuffer sb, jint offset, jboolean obj);
JNIEXPORT void JNICALL StringBufferInsertChar(JNIEnv *env, jstringBuffer sb, jint offset, jchar obj);
JNIEXPORT void JNICALL StringBufferInsertInt(JNIEnv *env, jstringBuffer sb, jint offset, jint obj);
JNIEXPORT void JNICALL StringBufferInsertLong(JNIEnv *env, jstringBuffer sb, jint offset, jlong obj);
JNIEXPORT void JNICALL StringBufferInsertFloat(JNIEnv *env, jstringBuffer sb, jint offset, jfloat obj);
JNIEXPORT void JNICALL StringBufferInsertDouble(JNIEnv *env, jstringBuffer sb, jint offset, jdouble obj);

/**
 * @brief Finds the index of a substring or UTF-8 string in the StringBuffer.
 */
JNIEXPORT jint JNICALL StringBufferIndexOf(JNIEnv *env, jstringBuffer sb, jstring str);
JNIEXPORT jint JNICALL StringBufferIndexOfUTF(JNIEnv *env, jstringBuffer sb, const char* str);
JNIEXPORT jint JNICALL StringBufferIndexOfI(JNIEnv *env, jstringBuffer sb, jstring str, jint fromIndex);
JNIEXPORT jint JNICALL StringBufferIndexOfIUTF(JNIEnv *env, jstringBuffer sb, const char* str, jint fromIndex);

/**
 * @brief Finds the last index of a substring or UTF-8 string in the StringBuffer.
 */
JNIEXPORT jint JNICALL StringBufferLastIndexOf(JNIEnv *env, jstringBuffer sb, jstring str);
JNIEXPORT jint JNICALL StringBufferLastIndexOfUTF(JNIEnv *env, jstringBuffer sb, const char* str);
JNIEXPORT jint JNICALL StringBufferLastIndexOfI(JNIEnv *env, jstringBuffer sb, jstring str, jint fromIndex);
JNIEXPORT jint JNICALL StringBufferLastIndexOfIUTF(JNIEnv *env, jstringBuffer sb, const char* str, jint fromIndex);

/**
 * @brief Reverses the contents of the StringBuffer.
 */
JNIEXPORT void JNICALL StringBufferReverse(JNIEnv *env, jstringBuffer sb);

/** @} */ // end of JNI_StringBuffer group

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRINGBUFFER_H__ */
