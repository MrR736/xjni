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

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobject JNICALL NewStringBuilder(JNIEnv *env);
JNIEXPORT jobject JNICALL NewStringBuilderCapacity(JNIEnv *env,jint capacity);
JNIEXPORT jobject JNICALL NewStringBuilderString(JNIEnv *env,jstring str);
JNIEXPORT jobject JNICALL NewStringBuilderStringUTF(JNIEnv *env,const char* str);

JNIEXPORT jstring JNICALL StringBuilderToString(JNIEnv *env, jobject sb);

/* Returns heap-allocated modified UTF-8. Caller MUST free(). */
JNIEXPORT char* JNICALL StringBuilderToStringUTF(JNIEnv *env, jobject sb);

JNIEXPORT void JNICALL StringBuilderAppendString(JNIEnv *env, jobject sb, jstring obj);
JNIEXPORT void JNICALL StringBuilderAppendStringUTF(JNIEnv *env, jobject sb, const char* str);
JNIEXPORT void JNICALL StringBuilderAppendObject(JNIEnv *env, jobject sb, jobject obj);
JNIEXPORT void JNICALL StringBuilderAppendStringBuffer(JNIEnv *env, jobject sb, jobject obj);
JNIEXPORT void JNICALL StringBuilderAppendBoolean(JNIEnv *env, jobject sb, jboolean obj);
JNIEXPORT void JNICALL StringBuilderAppendChar(JNIEnv *env, jobject sb, jchar obj);
JNIEXPORT void JNICALL StringBuilderAppendInt(JNIEnv *env, jobject sb, jint obj);
JNIEXPORT void JNICALL StringBuilderAppendLong(JNIEnv *env, jobject sb, jlong obj);
JNIEXPORT void JNICALL StringBuilderAppendFloat(JNIEnv *env, jobject sb, jfloat obj);
JNIEXPORT void JNICALL StringBuilderAppendDouble(JNIEnv *env, jobject sb, jdouble obj);

JNIEXPORT void JNICALL StringBuilderAppendCharIntInt(JNIEnv *env, jobject sb,jcharArray obj,jint offset,jint len);
JNIEXPORT void JNICALL StringBuilderAppendCodePoint(JNIEnv *env, jobject sb,jint codePoint);

JNIEXPORT void JNICALL StringBuilderDelete(JNIEnv *env, jobject sb,jint start,jint end);
JNIEXPORT void JNICALL StringBuilderDeleteCharAt(JNIEnv *env, jobject sb,jint index);

JNIEXPORT void JNICALL StringBuilderReplace(JNIEnv *env, jobject sb,jint start,jint end,jstring str);
JNIEXPORT void JNICALL StringBuilderReplaceUTF(JNIEnv *env, jobject sb,jint start,jint end,const char* str);

JNIEXPORT void JNICALL StringBuilderInsert(JNIEnv *env, jobject sb,jint index,jcharArray str,jint offset,jint len);
JNIEXPORT void JNICALL StringBuilderInsertString(JNIEnv *env,jobject sb,jint offset,jstring obj);
JNIEXPORT void JNICALL StringBuilderInsertStringUTF(JNIEnv *env, jobject sb, jint offset,const char* str);
JNIEXPORT void JNICALL StringBuilderInsertObject(JNIEnv *env, jobject sb, jint offset,jobject obj);
JNIEXPORT void JNICALL StringBuilderInsertCharArray(JNIEnv *env, jobject sb,jint offset,jcharArray obj);
JNIEXPORT void JNICALL StringBuilderInsertBoolean(JNIEnv *env, jobject sb,jint offset,jboolean obj);
JNIEXPORT void JNICALL StringBuilderInsertChar(JNIEnv *env, jobject sb,jint offset,jchar obj);
JNIEXPORT void JNICALL StringBuilderInsertInt(JNIEnv *env, jobject sb,jint offset,jint obj);
JNIEXPORT void JNICALL StringBuilderInsertLong(JNIEnv *env, jobject sb,jint offset,jlong obj);
JNIEXPORT void JNICALL StringBuilderInsertFloat(JNIEnv *env, jobject sb,jint offset,jfloat obj);
JNIEXPORT void JNICALL StringBuilderInsertDouble(JNIEnv *env, jobject sb,jint offset,jdouble obj);

JNIEXPORT jint JNICALL StringBuilderIndexOf(JNIEnv *env, jobject sb,jstring str);
JNIEXPORT jint JNICALL StringBuilderIndexOfUTF(JNIEnv *env,jobject sb,const char* str);
JNIEXPORT jint JNICALL StringBuilderIndexOfI(JNIEnv *env, jobject sb,jstring str, jint fromIndex);
JNIEXPORT jint JNICALL StringBuilderIndexOfIUTF(JNIEnv *env,jobject sb,const char* str, jint fromIndex);

JNIEXPORT jint JNICALL StringBuilderLastIndexOf(JNIEnv *env, jobject sb,jstring str);
JNIEXPORT jint JNICALL StringBuilderLastIndexOfUTF(JNIEnv *env,jobject sb,const char* str);
JNIEXPORT jint JNICALL StringBuilderLastIndexOfI(JNIEnv *env, jobject sb,jstring str, jint fromIndex);
JNIEXPORT jint JNICALL StringBuilderLastIndexOfIUTF(JNIEnv *env,jobject sb,const char* str, jint fromIndex);

JNIEXPORT void JNICALL StringBuilderReverse(JNIEnv *env, jobject sb);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRINGBUILDER_H__ */
