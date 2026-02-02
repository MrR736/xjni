/**
 * xjni_stringwriter.h: Extern JNI String Writer Utility
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

#ifndef __XJNI_STRINGWRITER_H__
#define __XJNI_STRINGWRITER_H__

#include <jni.h>

typedef jobject jstringWriter;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstringWriter JNICALL NewStringWriter(JNIEnv *env);
JNIEXPORT jstringWriter JNICALL NewStringWriterInitialSize(JNIEnv *env,jint initialSize);
JNIEXPORT jstring JNICALL StringWriterToString(JNIEnv *env, jstringWriter sw);

/* Returns heap-allocated modified UTF-8. Caller MUST free(). */
JNIEXPORT char* JNICALL StringWriterToStringUTF(JNIEnv *env, jstringWriter sw);

JNIEXPORT void JNICALL StringWriterWriteCharArrayIntInt(JNIEnv *env, jstringWriter sw,jcharArray obj,jint offset,jint len);
JNIEXPORT void JNICALL StringWriterWriteInt(JNIEnv *env, jstringWriter sw,jint len);
JNIEXPORT void JNICALL StringWriterWriteString(JNIEnv *env, jstringWriter sw,jstring str);
JNIEXPORT void JNICALL StringWriterWriteStringIntInt(JNIEnv *env, jstringWriter sw,jstring str,jint offset,jint len);

JNIEXPORT void JNICALL StringWriterFlush(JNIEnv *env, jstringWriter sw);

JNIEXPORT void JNICALL StringWriterAppendChar(JNIEnv *env, jstringWriter sb,jchar obj);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRINGWRITER_H__ */
