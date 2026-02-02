/**
 * xjni_arrayfield.h: Extern JNI Array Field Utility
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

#ifndef __XJNI_ARRAYFIELD_H__
#define __XJNI_ARRAYFIELD_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobjectArray JNICALL GetObjectArrayField(JNIEnv *env, jobject obj, jfieldID fid);
JNIEXPORT jobjectArray JNICALL GetStaticObjectArrayField(JNIEnv *env, jclass cls, jfieldID fid);
JNIEXPORT void JNICALL SetObjectArrayField(JNIEnv *env, jobject obj, jfieldID fid, jobjectArray value);
JNIEXPORT void JNICALL SetStaticObjectArrayField(JNIEnv *env, jclass cls, jfieldID fid, jobjectArray value);

JNIEXPORT jarray JNICALL GetArrayField(JNIEnv *env, jobject obj, jfieldID fid);
JNIEXPORT jarray JNICALL GetStaticArrayField(JNIEnv *env, jclass cls, jfieldID fid);
JNIEXPORT void JNICALL SetArrayField(JNIEnv *env, jobject obj, jfieldID fid, jarray value);
JNIEXPORT void JNICALL SetStaticArrayField(JNIEnv *env, jclass cls, jfieldID fid, jarray value);

JNIEXPORT jbyteArray JNICALL GetByteArrayField(JNIEnv *env, jobject obj, jfieldID fid);
JNIEXPORT jbyteArray JNICALL GetStaticByteArrayField(JNIEnv *env, jclass cls, jfieldID fid);
JNIEXPORT void JNICALL SetByteArrayField(JNIEnv *env, jobject obj, jfieldID fid, jbyteArray value);
JNIEXPORT void JNICALL SetStaticByteArrayField(JNIEnv *env, jclass cls, jfieldID fid, jbyteArray value);

JNIEXPORT jintArray JNICALL GetIntArrayField(JNIEnv *env, jobject obj, jfieldID fid);
JNIEXPORT jintArray JNICALL GetStaticIntArrayField(JNIEnv *env, jclass cls, jfieldID fid);
JNIEXPORT void JNICALL SetIntArrayField(JNIEnv *env, jobject obj, jfieldID fid, jintArray value);
JNIEXPORT void JNICALL SetStaticIntArrayField(JNIEnv *env, jclass cls, jfieldID fid, jintArray value);

JNIEXPORT jlongArray JNICALL GetLongArrayField(JNIEnv *env, jobject obj, jfieldID fid);
JNIEXPORT jlongArray JNICALL GetStaticLongArrayField(JNIEnv *env, jclass cls, jfieldID fid);
JNIEXPORT void JNICALL SetLongArrayField(JNIEnv *env, jobject obj, jfieldID fid, jlongArray value);
JNIEXPORT void JNICALL SetStaticLongArrayField(JNIEnv *env, jclass cls, jfieldID fid, jlongArray value);

JNIEXPORT jfloatArray JNICALL GetFloatArrayField(JNIEnv *env, jobject obj, jfieldID fid);
JNIEXPORT jfloatArray JNICALL GetStaticFloatArrayField(JNIEnv *env, jclass cls, jfieldID fid);
JNIEXPORT void JNICALL SetFloatArrayField(JNIEnv *env, jobject obj, jfieldID fid, jfloatArray value);
JNIEXPORT void JNICALL SetStaticFloatArrayField(JNIEnv *env, jclass cls, jfieldID fid, jfloatArray value);

JNIEXPORT jdoubleArray JNICALL GetDoubleArrayField(JNIEnv *env, jobject obj, jfieldID fid);
JNIEXPORT jdoubleArray JNICALL GetStaticDoubleArrayField(JNIEnv *env, jclass cls, jfieldID fid);
JNIEXPORT void JNICALL SetDoubleArrayField(JNIEnv *env, jobject obj, jfieldID fid, jdoubleArray value);
JNIEXPORT void JNICALL SetStaticDoubleArrayField(JNIEnv *env, jclass cls, jfieldID fid, jdoubleArray value);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_ARRAYFIELD_H__ */
