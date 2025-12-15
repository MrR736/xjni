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

extern jobjectArray GetObjectArrayField(JNIEnv *env, jobject obj, jfieldID fid);
extern jobjectArray GetStaticObjectArrayField(JNIEnv *env, jclass cls, jfieldID fid);
extern void SetObjectArrayField(JNIEnv *env, jobject obj, jfieldID fid, jobjectArray value);
extern void SetStaticObjectArrayField(JNIEnv *env, jclass cls, jfieldID fid, jobjectArray value);

extern jarray GetArrayField(JNIEnv *env, jobject obj, jfieldID fid);
extern jarray GetStaticArrayField(JNIEnv *env, jclass cls, jfieldID fid);
extern void SetArrayField(JNIEnv *env, jobject obj, jfieldID fid, jarray value);
extern void SetStaticArrayField(JNIEnv *env, jclass cls, jfieldID fid, jarray value);

extern jbyteArray GetByteArrayField(JNIEnv *env, jobject obj, jfieldID fid);
extern jbyteArray GetStaticByteArrayField(JNIEnv *env, jclass cls, jfieldID fid);
extern void SetByteArrayField(JNIEnv *env, jobject obj, jfieldID fid, jbyteArray value);
extern void SetStaticByteArrayField(JNIEnv *env, jclass cls, jfieldID fid, jbyteArray value);

extern jintArray GetIntArrayField(JNIEnv *env, jobject obj, jfieldID fid);
extern jintArray GetStaticIntArrayField(JNIEnv *env, jclass cls, jfieldID fid);
extern void SetIntArrayField(JNIEnv *env, jobject obj, jfieldID fid, jintArray value);
extern void SetStaticIntArrayField(JNIEnv *env, jclass cls, jfieldID fid, jintArray value);

extern jlongArray GetLongArrayField(JNIEnv *env, jobject obj, jfieldID fid);
extern jlongArray GetStaticLongArrayField(JNIEnv *env, jclass cls, jfieldID fid);
extern void SetLongArrayField(JNIEnv *env, jobject obj, jfieldID fid, jlongArray value);
extern void SetStaticLongArrayField(JNIEnv *env, jclass cls, jfieldID fid, jlongArray value);

extern jfloatArray GetFloatArrayField(JNIEnv *env, jobject obj, jfieldID fid);
extern jfloatArray GetStaticFloatArrayField(JNIEnv *env, jclass cls, jfieldID fid);
extern void SetFloatArrayField(JNIEnv *env, jobject obj, jfieldID fid, jfloatArray value);
extern void SetStaticFloatArrayField(JNIEnv *env, jclass cls, jfieldID fid, jfloatArray value);

extern jdoubleArray GetDoubleArrayField(JNIEnv *env, jobject obj, jfieldID fid);
extern jdoubleArray GetStaticDoubleArrayField(JNIEnv *env, jclass cls, jfieldID fid);
extern void SetDoubleArrayField(JNIEnv *env, jobject obj, jfieldID fid, jdoubleArray value);
extern void SetStaticDoubleArrayField(JNIEnv *env, jclass cls, jfieldID fid, jdoubleArray value);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_ARRAYFIELD_H__ */
