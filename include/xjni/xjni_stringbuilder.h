/**
 * @file xjni_stringbuilder.h
 * @brief Extern JNI StringBuilder Utility
 *
 * @author MrR736
 * @date 2025
 * @copyright GPL-3
 */

#ifndef __XJNI_STRINGBUILDER_H__
#define __XJNI_STRINGBUILDER_H__

#include <jni.h>
#include <xjni_stringbuffer.h>

/** @typedef jstringBuilder
 *  Represents a Java StringBuilder object.
 */
typedef jobject jstringBuilder;

#ifdef __cplusplus
extern "C" {
#endif

/** @name Constructors */
//@{
JNIEXPORT jstringBuilder JNICALL NewStringBuilder(JNIEnv *env);
JNIEXPORT jstringBuilder JNICALL NewStringBuilderCapacity(JNIEnv *env, jint capacity);
JNIEXPORT jstringBuilder JNICALL NewStringBuilderString(JNIEnv *env, jstring str);
JNIEXPORT jstringBuilder JNICALL NewStringBuilderStringUTF(JNIEnv *env, const char* str);
//@}

/** @name Conversion */
//@{
JNIEXPORT jstring JNICALL StringBuilderToString(JNIEnv *env, jstringBuilder sb);

/* Returns heap-allocated modified UTF-8. Caller MUST free(). */
JNIEXPORT char* JNICALL StringBuilderToStringUTF(JNIEnv *env, jstringBuilder sb);
//@}

/** @name Append Methods */
//@{
JNIEXPORT void JNICALL StringBuilderAppendString(JNIEnv *env, jstringBuilder sb, jstring obj);
JNIEXPORT void JNICALL StringBuilderAppendStringUTF(JNIEnv *env, jstringBuilder sb, const char* str);
JNIEXPORT void JNICALL StringBuilderAppendObject(JNIEnv *env, jstringBuilder sb, jobject obj);
JNIEXPORT void JNICALL StringBuilderAppendStringBuffer(JNIEnv *env, jstringBuilder sb, jstringBuffer obj);
JNIEXPORT void JNICALL StringBuilderAppendBoolean(JNIEnv *env, jstringBuilder sb, jboolean obj);
JNIEXPORT void JNICALL StringBuilderAppendChar(JNIEnv *env, jstringBuilder sb, jchar obj);
JNIEXPORT void JNICALL StringBuilderAppendInt(JNIEnv *env, jstringBuilder sb, jint obj);
JNIEXPORT void JNICALL StringBuilderAppendLong(JNIEnv *env, jstringBuilder sb, jlong obj);
JNIEXPORT void JNICALL StringBuilderAppendFloat(JNIEnv *env, jstringBuilder sb, jfloat obj);
JNIEXPORT void JNICALL StringBuilderAppendDouble(JNIEnv *env, jstringBuilder sb, jdouble obj);
JNIEXPORT void JNICALL StringBuilderAppendCharArrayIntInt(JNIEnv *env, jstringBuilder sb, jcharArray obj, jint offset, jint len);
JNIEXPORT void JNICALL StringBuilderAppendCodePoint(JNIEnv *env, jstringBuilder sb, jint codePoint);
//@}

/** @name Delete & Replace */
//@{
JNIEXPORT void JNICALL StringBuilderDelete(JNIEnv *env, jstringBuilder sb, jint start, jint end);
JNIEXPORT void JNICALL StringBuilderDeleteCharAt(JNIEnv *env, jstringBuilder sb, jint index);
JNIEXPORT void JNICALL StringBuilderReplace(JNIEnv *env, jstringBuilder sb, jint start, jint end, jstring str);
JNIEXPORT void JNICALL StringBuilderReplaceUTF(JNIEnv *env, jstringBuilder sb, jint start, jint end, const char* str);
//@}

/** @name Insert Methods */
//@{
JNIEXPORT void JNICALL StringBuilderInsertCharArrayIntInt(JNIEnv *env, jstringBuilder sb, jint index, jcharArray str, jint offset, jint len);
JNIEXPORT void JNICALL StringBuilderInsertString(JNIEnv *env, jstringBuilder sb, jint offset, jstring obj);
JNIEXPORT void JNICALL StringBuilderInsertStringUTF(JNIEnv *env, jstringBuilder sb, jint offset, const char* str);
JNIEXPORT void JNICALL StringBuilderInsertObject(JNIEnv *env, jstringBuilder sb, jint offset, jobject obj);
JNIEXPORT void JNICALL StringBuilderInsertCharArray(JNIEnv *env, jstringBuilder sb, jint offset, jcharArray obj);
JNIEXPORT void JNICALL StringBuilderInsertBoolean(JNIEnv *env, jstringBuilder sb, jint offset, jboolean obj);
JNIEXPORT void JNICALL StringBuilderInsertChar(JNIEnv *env, jstringBuilder sb, jint offset, jchar obj);
JNIEXPORT void JNICALL StringBuilderInsertInt(JNIEnv *env, jstringBuilder sb, jint offset, jint obj);
JNIEXPORT void JNICALL StringBuilderInsertLong(JNIEnv *env, jstringBuilder sb, jint offset, jlong obj);
JNIEXPORT void JNICALL StringBuilderInsertFloat(JNIEnv *env, jstringBuilder sb, jint offset, jfloat obj);
JNIEXPORT void JNICALL StringBuilderInsertDouble(JNIEnv *env, jstringBuilder sb, jint offset, jdouble obj);
//@}

/** @name Search Methods */
//@{
JNIEXPORT jint JNICALL StringBuilderIndexOf(JNIEnv *env, jstringBuilder sb, jstring str);
JNIEXPORT jint JNICALL StringBuilderIndexOfUTF(JNIEnv *env, jstringBuilder sb, const char* str);
JNIEXPORT jint JNICALL StringBuilderIndexOfI(JNIEnv *env, jstringBuilder sb, jstring str, jint fromIndex);
JNIEXPORT jint JNICALL StringBuilderIndexOfIUTF(JNIEnv *env, jstringBuilder sb, const char* str, jint fromIndex);
JNIEXPORT jint JNICALL StringBuilderLastIndexOf(JNIEnv *env, jstringBuilder sb, jstring str);
JNIEXPORT jint JNICALL StringBuilderLastIndexOfUTF(JNIEnv *env, jstringBuilder sb, const char* str);
JNIEXPORT jint JNICALL StringBuilderLastIndexOfI(JNIEnv *env, jstringBuilder sb, jstring str, jint fromIndex);
JNIEXPORT jint JNICALL StringBuilderLastIndexOfIUTF(JNIEnv *env, jstringBuilder sb, const char* str, jint fromIndex);
//@}

/** @name Miscellaneous */
//@{
JNIEXPORT void JNICALL StringBuilderReverse(JNIEnv *env, jstringBuilder sb);
//@}

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRINGBUILDER_H__ */
