/**
 * @file xjni_stringbuilder.h
 * @brief Extern JNI StringBuilder Utility
 *
 * @author MrR736
 * @date 2025
 * @copyright MIT
 */

#ifndef __XJNI_STRINGBUILDER_H__
#define __XJNI_STRINGBUILDER_H__

#include <jni.h>

#include "xjnidef.h"
#include "xjni_stringbuffer.h"

/** @typedef jstringBuilder
 *  Represents a Java StringBuilder object.
 */
typedef jobject jstringBuilder;

#ifdef __cplusplus
extern "C" {
#endif

/** @name Constructors */
//@{
XJNIAPI jstringBuilder XJNICALL NewStringBuilder(JNIEnv *env);
XJNIAPI jstringBuilder XJNICALL NewStringBuilderCapacity(JNIEnv *env, jint capacity);
XJNIAPI jstringBuilder XJNICALL NewStringBuilderString(JNIEnv *env, jstring str);
XJNIAPI jstringBuilder XJNICALL NewStringBuilderStringUTF(JNIEnv *env, const char* str);
//@}

/** @name Conversion */
//@{
XJNIAPI jstring XJNICALL StringBuilderToString(JNIEnv *env, jstringBuilder sb);

/* Returns heap-allocated modified UTF-8. Caller MUST free(). */
XJNIAPI char* XJNICALL StringBuilderToStringUTF(JNIEnv *env, jstringBuilder sb);
//@}

/** @name Append Methods */
//@{
XJNIAPI void XJNICALL StringBuilderAppendString(JNIEnv *env, jstringBuilder sb, jstring obj);
XJNIAPI void XJNICALL StringBuilderAppendStringUTF(JNIEnv *env, jstringBuilder sb, const char* str);
XJNIAPI void XJNICALL StringBuilderAppendObject(JNIEnv *env, jstringBuilder sb, jobject obj);
XJNIAPI void XJNICALL StringBuilderAppendStringBuffer(JNIEnv *env, jstringBuilder sb, jstringBuffer obj);
XJNIAPI void XJNICALL StringBuilderAppendBoolean(JNIEnv *env, jstringBuilder sb, jboolean obj);
XJNIAPI void XJNICALL StringBuilderAppendChar(JNIEnv *env, jstringBuilder sb, jchar obj);
XJNIAPI void XJNICALL StringBuilderAppendInt(JNIEnv *env, jstringBuilder sb, jint obj);
XJNIAPI void XJNICALL StringBuilderAppendLong(JNIEnv *env, jstringBuilder sb, jlong obj);
XJNIAPI void XJNICALL StringBuilderAppendFloat(JNIEnv *env, jstringBuilder sb, jfloat obj);
XJNIAPI void XJNICALL StringBuilderAppendDouble(JNIEnv *env, jstringBuilder sb, jdouble obj);
XJNIAPI void XJNICALL StringBuilderAppendCharArrayIntInt(JNIEnv *env, jstringBuilder sb, jcharArray obj, jint offset, jint len);
XJNIAPI void XJNICALL StringBuilderAppendCodePoint(JNIEnv *env, jstringBuilder sb, jint codePoint);
//@}

/** @name Delete & Replace */
//@{
XJNIAPI void XJNICALL StringBuilderDelete(JNIEnv *env, jstringBuilder sb, jint start, jint end);
XJNIAPI void XJNICALL StringBuilderDeleteCharAt(JNIEnv *env, jstringBuilder sb, jint index);
XJNIAPI void XJNICALL StringBuilderReplace(JNIEnv *env, jstringBuilder sb, jint start, jint end, jstring str);
XJNIAPI void XJNICALL StringBuilderReplaceUTF(JNIEnv *env, jstringBuilder sb, jint start, jint end, const char* str);
//@}

/** @name Insert Methods */
//@{
XJNIAPI void XJNICALL StringBuilderInsertCharArrayIntInt(JNIEnv *env, jstringBuilder sb, jint index, jcharArray str, jint offset, jint len);
XJNIAPI void XJNICALL StringBuilderInsertString(JNIEnv *env, jstringBuilder sb, jint offset, jstring obj);
XJNIAPI void XJNICALL StringBuilderInsertStringUTF(JNIEnv *env, jstringBuilder sb, jint offset, const char* str);
XJNIAPI void XJNICALL StringBuilderInsertObject(JNIEnv *env, jstringBuilder sb, jint offset, jobject obj);
XJNIAPI void XJNICALL StringBuilderInsertCharArray(JNIEnv *env, jstringBuilder sb, jint offset, jcharArray obj);
XJNIAPI void XJNICALL StringBuilderInsertBoolean(JNIEnv *env, jstringBuilder sb, jint offset, jboolean obj);
XJNIAPI void XJNICALL StringBuilderInsertChar(JNIEnv *env, jstringBuilder sb, jint offset, jchar obj);
XJNIAPI void XJNICALL StringBuilderInsertInt(JNIEnv *env, jstringBuilder sb, jint offset, jint obj);
XJNIAPI void XJNICALL StringBuilderInsertLong(JNIEnv *env, jstringBuilder sb, jint offset, jlong obj);
XJNIAPI void XJNICALL StringBuilderInsertFloat(JNIEnv *env, jstringBuilder sb, jint offset, jfloat obj);
XJNIAPI void XJNICALL StringBuilderInsertDouble(JNIEnv *env, jstringBuilder sb, jint offset, jdouble obj);
//@}

/** @name Search Methods */
//@{
XJNIAPI jint XJNICALL StringBuilderIndexOf(JNIEnv *env, jstringBuilder sb, jstring str);
XJNIAPI jint XJNICALL StringBuilderIndexOfUTF(JNIEnv *env, jstringBuilder sb, const char* str);
XJNIAPI jint XJNICALL StringBuilderIndexOfI(JNIEnv *env, jstringBuilder sb, jstring str, jint fromIndex);
XJNIAPI jint XJNICALL StringBuilderIndexOfIUTF(JNIEnv *env, jstringBuilder sb, const char* str, jint fromIndex);
XJNIAPI jint XJNICALL StringBuilderLastIndexOf(JNIEnv *env, jstringBuilder sb, jstring str);
XJNIAPI jint XJNICALL StringBuilderLastIndexOfUTF(JNIEnv *env, jstringBuilder sb, const char* str);
XJNIAPI jint XJNICALL StringBuilderLastIndexOfI(JNIEnv *env, jstringBuilder sb, jstring str, jint fromIndex);
XJNIAPI jint XJNICALL StringBuilderLastIndexOfIUTF(JNIEnv *env, jstringBuilder sb, const char* str, jint fromIndex);
//@}

/** @name Miscellaneous */
//@{
XJNIAPI void XJNICALL StringBuilderReverse(JNIEnv *env, jstringBuilder sb);
//@}

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRINGBUILDER_H__ */
