/**
 * xjni_stringbuilder.h: Extern JNI String Builder Utility
 *
 * Copyright (C) 2025 MrR736 <MrR736@users.github.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * The complete text of the GNU General Public License
 * can be found in /usr/share/common-licenses/GPL-3 file.
 */

#ifndef __XJNI_STRINGBUILDER_H__
#define __XJNI_STRINGBUILDER_H__

#include <jni.h>

typedef jobject jstringBuilder;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstringBuilder JNICALL NewStringBuilder(JNIEnv *env);
JNIEXPORT jstringBuilder JNICALL NewStringBuilderCapacity(JNIEnv *env,jint capacity);
JNIEXPORT jstringBuilder JNICALL NewStringBuilderString(JNIEnv *env,jstring str);
JNIEXPORT jstringBuilder JNICALL NewStringBuilderStringUTF(JNIEnv *env,const char* str);

JNIEXPORT jstring JNICALL StringBuilderToString(JNIEnv *env, jstringBuilder sb);

/* Returns heap-allocated modified UTF-8. Caller MUST free(). */
JNIEXPORT char* JNICALL StringBuilderToStringUTF(JNIEnv *env, jstringBuilder sb);

JNIEXPORT void JNICALL StringBuilderAppendString(JNIEnv *env, jstringBuilder sb, jstring obj);
JNIEXPORT void JNICALL StringBuilderAppendStringUTF(JNIEnv *env, jstringBuilder sb, const char* str);
JNIEXPORT void JNICALL StringBuilderAppendObject(JNIEnv *env, jstringBuilder sb, jobject obj);
JNIEXPORT void JNICALL StringBuilderAppendStringBuffer(JNIEnv *env, jstringBuilder sb, jobject obj);
JNIEXPORT void JNICALL StringBuilderAppendBoolean(JNIEnv *env, jstringBuilder sb, jboolean obj);
JNIEXPORT void JNICALL StringBuilderAppendChar(JNIEnv *env, jstringBuilder sb, jchar obj);
JNIEXPORT void JNICALL StringBuilderAppendInt(JNIEnv *env, jstringBuilder sb, jint obj);
JNIEXPORT void JNICALL StringBuilderAppendLong(JNIEnv *env, jstringBuilder sb, jlong obj);
JNIEXPORT void JNICALL StringBuilderAppendFloat(JNIEnv *env, jstringBuilder sb, jfloat obj);
JNIEXPORT void JNICALL StringBuilderAppendDouble(JNIEnv *env, jstringBuilder sb, jdouble obj);

JNIEXPORT void JNICALL StringBuilderAppendCharIntInt(JNIEnv *env, jstringBuilder sb,jcharArray obj,jint offset,jint len);
JNIEXPORT void JNICALL StringBuilderAppendCodePoint(JNIEnv *env, jstringBuilder sb,jint codePoint);

JNIEXPORT void JNICALL StringBuilderDelete(JNIEnv *env, jstringBuilder sb,jint start,jint end);
JNIEXPORT void JNICALL StringBuilderDeleteCharAt(JNIEnv *env, jstringBuilder sb,jint index);

JNIEXPORT void JNICALL StringBuilderReplace(JNIEnv *env, jstringBuilder sb,jint start,jint end,jstring str);
JNIEXPORT void JNICALL StringBuilderReplaceUTF(JNIEnv *env, jstringBuilder sb,jint start,jint end,const char* str);

JNIEXPORT void JNICALL StringBuilderInsert(JNIEnv *env, jstringBuilder sb,jint index,jcharArray str,jint offset,jint len);
JNIEXPORT void JNICALL StringBuilderInsertString(JNIEnv *env,jstringBuilder sb,jint offset,jstring obj);
JNIEXPORT void JNICALL StringBuilderInsertStringUTF(JNIEnv *env, jstringBuilder sb, jint offset,const char* str);
JNIEXPORT void JNICALL StringBuilderInsertObject(JNIEnv *env, jstringBuilder sb, jint offset,jobject obj);
JNIEXPORT void JNICALL StringBuilderInsertCharArray(JNIEnv *env, jstringBuilder sb,jint offset,jcharArray obj);
JNIEXPORT void JNICALL StringBuilderInsertBoolean(JNIEnv *env, jstringBuilder sb,jint offset,jboolean obj);
JNIEXPORT void JNICALL StringBuilderInsertChar(JNIEnv *env, jstringBuilder sb,jint offset,jchar obj);
JNIEXPORT void JNICALL StringBuilderInsertInt(JNIEnv *env, jstringBuilder sb,jint offset,jint obj);
JNIEXPORT void JNICALL StringBuilderInsertLong(JNIEnv *env, jstringBuilder sb,jint offset,jlong obj);
JNIEXPORT void JNICALL StringBuilderInsertFloat(JNIEnv *env, jstringBuilder sb,jint offset,jfloat obj);
JNIEXPORT void JNICALL StringBuilderInsertDouble(JNIEnv *env, jstringBuilder sb,jint offset,jdouble obj);

JNIEXPORT jint JNICALL StringBuilderIndexOf(JNIEnv *env, jstringBuilder sb,jstring str);
JNIEXPORT jint JNICALL StringBuilderIndexOfUTF(JNIEnv *env,jstringBuilder sb,const char* str);
JNIEXPORT jint JNICALL StringBuilderIndexOfI(JNIEnv *env, jstringBuilder sb,jstring str, jint fromIndex);
JNIEXPORT jint JNICALL StringBuilderIndexOfIUTF(JNIEnv *env,jstringBuilder sb,const char* str, jint fromIndex);

JNIEXPORT jint JNICALL StringBuilderLastIndexOf(JNIEnv *env, jstringBuilder sb,jstring str);
JNIEXPORT jint JNICALL StringBuilderLastIndexOfUTF(JNIEnv *env,jstringBuilder sb,const char* str);
JNIEXPORT jint JNICALL StringBuilderLastIndexOfI(JNIEnv *env, jstringBuilder sb,jstring str, jint fromIndex);
JNIEXPORT jint JNICALL StringBuilderLastIndexOfIUTF(JNIEnv *env,jstringBuilder sb,const char* str, jint fromIndex);

JNIEXPORT void JNICALL StringBuilderReverse(JNIEnv *env, jstringBuilder sb);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRINGBUILDER_H__ */
