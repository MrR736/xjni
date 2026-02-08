/**
 * @file xjni_arrayfield.h
 * @brief Extern JNI Array Field Utility
 *
 * Provides helper functions to get and set Java array fields (object and primitive arrays)
 * for both instance and static fields using JNI.
 *
 * @author MrR736
 * @date 2025
 * @copyright GPL-3
 */

#ifndef __XJNI_ARRAYFIELD_H__
#define __XJNI_ARRAYFIELD_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @name Object Array Field Accessors
 *  Get and set instance or static object array fields
 */
//@{
JNIEXPORT jobjectArray JNICALL GetObjectArrayField(JNIEnv *env, jobject obj, jfieldID fid);
JNIEXPORT jobjectArray JNICALL GetStaticObjectArrayField(JNIEnv *env, jclass cls, jfieldID fid);
JNIEXPORT void JNICALL SetObjectArrayField(JNIEnv *env, jobject obj, jfieldID fid, jobjectArray value);
JNIEXPORT void JNICALL SetStaticObjectArrayField(JNIEnv *env, jclass cls, jfieldID fid, jobjectArray value);
//@}

/** @name Generic Array Field Accessors
 *  Get and set instance or static fields of any array type
 */
//@{
JNIEXPORT jarray JNICALL GetArrayField(JNIEnv *env, jobject obj, jfieldID fid);
JNIEXPORT jarray JNICALL GetStaticArrayField(JNIEnv *env, jclass cls, jfieldID fid);
JNIEXPORT void JNICALL SetArrayField(JNIEnv *env, jobject obj, jfieldID fid, jarray value);
JNIEXPORT void JNICALL SetStaticArrayField(JNIEnv *env, jclass cls, jfieldID fid, jarray value);
//@}

/** @name Byte Array Field Accessors */
//@{
JNIEXPORT jbyteArray JNICALL GetByteArrayField(JNIEnv *env, jobject obj, jfieldID fid);
JNIEXPORT jbyteArray JNICALL GetStaticByteArrayField(JNIEnv *env, jclass cls, jfieldID fid);
JNIEXPORT void JNICALL SetByteArrayField(JNIEnv *env, jobject obj, jfieldID fid, jbyteArray value);
JNIEXPORT void JNICALL SetStaticByteArrayField(JNIEnv *env, jclass cls, jfieldID fid, jbyteArray value);
//@}

/** @name Int Array Field Accessors */
//@{
JNIEXPORT jintArray JNICALL GetIntArrayField(JNIEnv *env, jobject obj, jfieldID fid);
JNIEXPORT jintArray JNICALL GetStaticIntArrayField(JNIEnv *env, jclass cls, jfieldID fid);
JNIEXPORT void JNICALL SetIntArrayField(JNIEnv *env, jobject obj, jfieldID fid, jintArray value);
JNIEXPORT void JNICALL SetStaticIntArrayField(JNIEnv *env, jclass cls, jfieldID fid, jintArray value);
//@}

/** @name Long Array Field Accessors */
//@{
JNIEXPORT jlongArray JNICALL GetLongArrayField(JNIEnv *env, jobject obj, jfieldID fid);
JNIEXPORT jlongArray JNICALL GetStaticLongArrayField(JNIEnv *env, jclass cls, jfieldID fid);
JNIEXPORT void JNICALL SetLongArrayField(JNIEnv *env, jobject obj, jfieldID fid, jlongArray value);
JNIEXPORT void JNICALL SetStaticLongArrayField(JNIEnv *env, jclass cls, jfieldID fid, jlongArray value);
//@}

/** @name Float Array Field Accessors */
//@{
JNIEXPORT jfloatArray JNICALL GetFloatArrayField(JNIEnv *env, jobject obj, jfieldID fid);
JNIEXPORT jfloatArray JNICALL GetStaticFloatArrayField(JNIEnv *env, jclass cls, jfieldID fid);
JNIEXPORT void JNICALL SetFloatArrayField(JNIEnv *env, jobject obj, jfieldID fid, jfloatArray value);
JNIEXPORT void JNICALL SetStaticFloatArrayField(JNIEnv *env, jclass cls, jfieldID fid, jfloatArray value);
//@}

/** @name Double Array Field Accessors */
//@{
JNIEXPORT jdoubleArray JNICALL GetDoubleArrayField(JNIEnv *env, jobject obj, jfieldID fid);
JNIEXPORT jdoubleArray JNICALL GetStaticDoubleArrayField(JNIEnv *env, jclass cls, jfieldID fid);
JNIEXPORT void JNICALL SetDoubleArrayField(JNIEnv *env, jobject obj, jfieldID fid, jdoubleArray value);
JNIEXPORT void JNICALL SetStaticDoubleArrayField(JNIEnv *env, jclass cls, jfieldID fid, jdoubleArray value);
//@}

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_ARRAYFIELD_H__ */
