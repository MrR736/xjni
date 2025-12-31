/**
 * xjni_new.h: Extern JNI New Utility
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

#ifndef __XJNI_NEW_H__
#define __XJNI_NEW_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Create a new object array in Java from the provided C data. */
JNIEXPORT jobjectArray JNICALL xjni_NewObjectArray(JNIEnv* env, jclass clz, jsize len);

/* Create a new byte array in Java from the provided C data. */
JNIEXPORT jbyteArray JNICALL xjni_NewByteArray(JNIEnv* env, const jbyte* data, jsize len);

/* Create a new integer array in Java from the provided C data. */
JNIEXPORT jintArray JNICALL xjni_NewIntArray(JNIEnv* env, const jint* data, jsize len);

/* Create a new long array in Java from the provided C data. */
JNIEXPORT jlongArray JNICALL xjni_NewLongArray(JNIEnv* env, const jlong* data, jsize len);

/* Create a new float array in Java from the provided C data. */
JNIEXPORT jfloatArray JNICALL xjni_NewFloatArray(JNIEnv* env, const float* data, jsize len);

/* Create a new double array in Java from the provided C data. */
JNIEXPORT jdoubleArray JNICALL xjni_NewDoubleArray(JNIEnv* env, const double* data, jsize len);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_NEW_H__ */
