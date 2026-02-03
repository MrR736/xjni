/**
 * xjni_stringbuffer.h: Extern JNI String Buffer Utility
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

#ifndef __XJNI_STRINGBUFFER_H__
#define __XJNI_STRINGBUFFER_H__

#include <jni.h>

#ifdef __cplusplus
typedef _jobject* jstringBuffer;
#else
typedef jobject jstringBuffer;
#endif

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstringBuffer JNICALL NewStringBuffer(JNIEnv *env);
JNIEXPORT jstringBuffer JNICALL NewStringBufferCapacity(JNIEnv *env,jint capacity);
JNIEXPORT jstringBuffer JNICALL NewStringBufferString(JNIEnv *env,jstring str);
JNIEXPORT jstringBuffer JNICALL NewStringBufferStringUTF(JNIEnv *env,const char* str);

JNIEXPORT jstring JNICALL StringBufferToString(JNIEnv *env, jstringBuffer sb);

/* Returns heap-allocated modified UTF-8. Caller MUST free(). */
JNIEXPORT char* JNICALL StringBufferToStringUTF(JNIEnv *env, jstringBuffer sb);

JNIEXPORT jstring JNICALL StringBufferSubString(JNIEnv *env, jstringBuffer sb,jint start);
JNIEXPORT jstring JNICALL StringBufferSubStringIntInt(JNIEnv *env, jstringBuffer sb,jint start,jint end);
JNIEXPORT jobject JNICALL StringBufferSubSequence(JNIEnv *env, jstringBuffer sb,jint start,jint end);

JNIEXPORT void JNICALL StringBufferAppendString(JNIEnv *env, jstringBuffer sb, jstring obj);
JNIEXPORT void JNICALL StringBufferAppendStringUTF(JNIEnv *env, jstringBuffer sb, const char* str);
JNIEXPORT void JNICALL StringBufferAppendObject(JNIEnv *env, jstringBuffer sb, jobject obj);
JNIEXPORT void JNICALL StringBufferAppendStringBuffer(JNIEnv *env, jstringBuffer sb, jstringBuffer obj);
JNIEXPORT void JNICALL StringBufferAppendBoolean(JNIEnv *env, jstringBuffer sb, jboolean obj);
JNIEXPORT void JNICALL StringBufferAppendChar(JNIEnv *env, jstringBuffer sb, jchar obj);
JNIEXPORT void JNICALL StringBufferAppendCharArray(JNIEnv *env, jstringBuffer sb, jcharArray obj);
JNIEXPORT void JNICALL StringBufferAppendInt(JNIEnv *env, jstringBuffer sb, jint obj);
JNIEXPORT void JNICALL StringBufferAppendLong(JNIEnv *env, jstringBuffer sb, jlong obj);
JNIEXPORT void JNICALL StringBufferAppendFloat(JNIEnv *env, jstringBuffer sb, jfloat obj);
JNIEXPORT void JNICALL StringBufferAppendDouble(JNIEnv *env, jstringBuffer sb, jdouble obj);
JNIEXPORT void JNICALL StringBufferAppendCharArrayIntInt(JNIEnv *env, jstringBuffer sb,jcharArray obj,jint offset,jint len);
JNIEXPORT void JNICALL StringBufferAppendCodePoint(JNIEnv *env, jstringBuffer sb,jint codePoint);

JNIEXPORT jint JNICALL StringBufferLength(JNIEnv *env, jstringBuffer sb);
JNIEXPORT jint JNICALL StringBufferCapacity(JNIEnv *env, jstringBuffer sb);

JNIEXPORT void JNICALL StringBufferEnsureCapacity(JNIEnv *env,jstringBuffer sb,jint minimumCapacity);
JNIEXPORT void JNICALL StringBufferSetLength(JNIEnv *env,jstringBuffer sb,jint newLength);

JNIEXPORT jchar JNICALL StringBufferCharAt(JNIEnv *env,jstringBuffer sb,jint index);

JNIEXPORT jint JNICALL StringBufferCodePointAt(JNIEnv *env,jstringBuffer sb,jint index);
JNIEXPORT jint JNICALL StringBufferCodePointBefore(JNIEnv *env,jstringBuffer sb,jint index);
JNIEXPORT jint JNICALL StringBufferCodePointCount(JNIEnv *env,jstringBuffer sb,jint beginIndex,jint endIndex);
JNIEXPORT jint JNICALL StringBufferOffsetByCodePoints(JNIEnv *env,jstringBuffer sb,jint index,jint codePointOffset);

JNIEXPORT void JNICALL StringBufferTrimToSize(JNIEnv *env,jstringBuffer sb);

JNIEXPORT void JNICALL StringBufferGetChars(JNIEnv *env,jstringBuffer sb,jint srcBegin,jint srcEnd,jcharArray dst,jint dstBegin);
JNIEXPORT void JNICALL StringBufferSetCharAt(JNIEnv *env,jstringBuffer sb,jint index,jchar ch);

JNIEXPORT void JNICALL StringBufferDelete(JNIEnv *env, jstringBuffer sb,jint start,jint end);
JNIEXPORT void JNICALL StringBufferDeleteCharAt(JNIEnv *env, jstringBuffer sb,jint index);

JNIEXPORT void JNICALL StringBufferReplace(JNIEnv *env, jstringBuffer sb,jint start,jint end,jstring str);
JNIEXPORT void JNICALL StringBufferReplaceUTF(JNIEnv *env, jstringBuffer sb,jint start,jint end,const char* str);

JNIEXPORT void JNICALL StringBufferInsertCharArrayIntInt(JNIEnv *env,jstringBuffer sb,jint index,jcharArray str,jint offset,jint len);
JNIEXPORT void JNICALL StringBufferInsertString(JNIEnv *env,jstringBuffer sb,jint offset,jstring obj);
JNIEXPORT void JNICALL StringBufferInsertStringUTF(JNIEnv *env, jstringBuffer sb, jint offset,const char* str);
JNIEXPORT void JNICALL StringBufferInsertObject(JNIEnv *env, jstringBuffer sb, jint offset,jobject obj);
JNIEXPORT void JNICALL StringBufferInsertCharArray(JNIEnv *env, jstringBuffer sb,jint offset,jcharArray obj);
JNIEXPORT void JNICALL StringBufferInsertBoolean(JNIEnv *env, jstringBuffer sb,jint offset,jboolean obj);
JNIEXPORT void JNICALL StringBufferInsertChar(JNIEnv *env, jstringBuffer sb,jint offset,jchar obj);
JNIEXPORT void JNICALL StringBufferInsertInt(JNIEnv *env, jstringBuffer sb,jint offset,jint obj);
JNIEXPORT void JNICALL StringBufferInsertLong(JNIEnv *env, jstringBuffer sb,jint offset,jlong obj);
JNIEXPORT void JNICALL StringBufferInsertFloat(JNIEnv *env, jstringBuffer sb,jint offset,jfloat obj);
JNIEXPORT void JNICALL StringBufferInsertDouble(JNIEnv *env, jstringBuffer sb,jint offset,jdouble obj);

JNIEXPORT jint JNICALL StringBufferIndexOf(JNIEnv *env, jstringBuffer sb,jstring str);
JNIEXPORT jint JNICALL StringBufferIndexOfUTF(JNIEnv *env,jstringBuffer sb,const char* str);
JNIEXPORT jint JNICALL StringBufferIndexOfI(JNIEnv *env, jstringBuffer sb,jstring str, jint fromIndex);
JNIEXPORT jint JNICALL StringBufferIndexOfIUTF(JNIEnv *env,jstringBuffer sb,const char* str, jint fromIndex);

JNIEXPORT jint JNICALL StringBufferLastIndexOf(JNIEnv *env, jstringBuffer sb,jstring str);
JNIEXPORT jint JNICALL StringBufferLastIndexOfUTF(JNIEnv *env,jstringBuffer sb,const char* str);
JNIEXPORT jint JNICALL StringBufferLastIndexOfI(JNIEnv *env, jstringBuffer sb,jstring str, jint fromIndex);
JNIEXPORT jint JNICALL StringBufferLastIndexOfIUTF(JNIEnv *env,jstringBuffer sb,const char* str, jint fromIndex);

JNIEXPORT void JNICALL StringBufferReverse(JNIEnv *env, jstringBuffer sb);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRINGBUFFER_H__ */
