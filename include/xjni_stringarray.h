/**
 * xjni_stringarray.h: Extern JNI String Array Utility
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

#ifndef __XJNI_STRINGARRAY_H__
#define __XJNI_STRINGARRAY_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

// String[] - Access and release functions for Java String[].

JNIEXPORT jobjectArray JNICALL NewStringUTFArray(JNIEnv *env, const char **utf,jsize count);
JNIEXPORT const char** JNICALL GetStringUTFArrayChars(JNIEnv *env, jobjectArray array,jboolean *isCopy);
JNIEXPORT void JNICALL ReleaseStringUTFArrayChars(JNIEnv *env, jobjectArray array,const char **elements, jint mode);
JNIEXPORT void JNICALL GetStringUTFArrayRegion(JNIEnv *env,jobjectArray str,jsize start,jsize len,char **buf);
JNIEXPORT void JNICALL SetStringUTFArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const char **buf);
JNIEXPORT jsize JNICALL GetStringUTFArrayLength(JNIEnv *env, jobjectArray str);

JNIEXPORT jobjectArray JNICALL NewStringArray(JNIEnv *env, const jchar **unicode, jsize len, jsize n);
JNIEXPORT jsize JNICALL GetStringArrayLength(JNIEnv *env, jobjectArray str);
JNIEXPORT const jchar** JNICALL GetStringArrayChars(JNIEnv *env, jobjectArray str, jboolean *isCopy);
JNIEXPORT void JNICALL ReleaseStringArrayChars(JNIEnv *env, jobjectArray str, const jchar **chars);
JNIEXPORT void JNICALL GetStringArrayRegion(JNIEnv *env,jobjectArray str,jsize start,jsize len,jchar **buf);
JNIEXPORT void JNICALL SetStringArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jchar **buf);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRINGARRAY_H__ */
