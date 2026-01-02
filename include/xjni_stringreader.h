/**
 * xjni_stringreader.h: Extern JNI String Reader Utility
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

#ifndef __XJNI_STRINGREADER_H__
#define __XJNI_STRINGREADER_H__

#include <jni.h>

typedef jobject jstringReader;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstringReader JNICALL NewStringReader(JNIEnv *env,jstring str);
JNIEXPORT jstringReader JNICALL NewStringReaderUTF(JNIEnv *env,const char* str);

JNIEXPORT jstring JNICALL StringReaderToString(JNIEnv *env, jstringReader sr);

/* Returns heap-allocated modified UTF-8. Caller MUST free(). */
JNIEXPORT char* JNICALL StringReaderToStringUTF(JNIEnv *env, jstringReader sr);

JNIEXPORT void JNICALL StringReaderEnsureOpen(JNIEnv *env, jstringReader sr);

JNIEXPORT jint JNICALL StringReaderRead(JNIEnv *env, jstringReader sr);
JNIEXPORT jint JNICALL StringReaderReadCharArrayIntInt(JNIEnv *env,jstringReader sr,jcharArray obj,jint offset,jint len);

JNIEXPORT jlong JNICALL StringReaderReadSkip(JNIEnv *env,jstringReader sr,jlong ns);

JNIEXPORT jboolean JNICALL StringReaderReadReady(JNIEnv *env,jstringReader sr);

JNIEXPORT jboolean JNICALL StringReaderReadMarkSupported(JNIEnv *env,jstringReader sr);
JNIEXPORT void JNICALL StringReaderMark(JNIEnv *env,jstringReader sr,jint readAheadLimit);

JNIEXPORT void JNICALL StringReaderReset(JNIEnv *env,jstringReader sr);

JNIEXPORT void JNICALL StringReaderClose(JNIEnv *env,jstringReader sr);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRINGREADER_H__ */
