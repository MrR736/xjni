/**
 * @file xjni2d.h
 * @brief Extern JNI 2D Array Utility
 *
 * Provides creation, access, release, and region functions for
 * Java 2D arrays of all primitive types and strings.
 *
 * @author MrR736
 * @date 2026
 * @copyright MIT
 */

#ifndef __XJNI2D_H__
#define __XJNI2D_H__

#include <jni.h>
#include "xjnidef.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup JNI_2DArray Java 2D Array Utilities
 *  Functions to create, access, and manipulate Java 2D arrays from native C/C++.
 *  @{
 */

/** @name Byte 2D Array Utility **/
//@{
XJNIAPI jobjectArray XJNICALL NewByte2DArray(JNIEnv *env, jsize row, jsize col);
XJNIAPI jbyte** XJNICALL GetByte2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
XJNIAPI void XJNICALL ReleaseByte2DArrayElements(JNIEnv *env, jobjectArray array, jbyte **elements, jint mode);
XJNIAPI void XJNICALL SetByte2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jbyte **buf);
XJNIAPI void XJNICALL GetByte2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jbyte **buf);
//@}

/** @name Int 2D Array Utility **/
//@{
XJNIAPI jobjectArray XJNICALL NewInt2DArray(JNIEnv *env, jsize row, jsize col);
XJNIAPI jint** XJNICALL GetInt2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
XJNIAPI void XJNICALL ReleaseInt2DArrayElements(JNIEnv *env, jobjectArray array, jint **elements, jint mode);
XJNIAPI void XJNICALL SetInt2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jint **buf);
XJNIAPI void XJNICALL GetInt2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jint **buf);
//@}

/** @name Long 2D Array Utility **/
//@{
XJNIAPI jobjectArray XJNICALL NewLong2DArray(JNIEnv *env, jsize row, jsize col);
XJNIAPI jlong** XJNICALL GetLong2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
XJNIAPI void XJNICALL ReleaseLong2DArrayElements(JNIEnv *env, jobjectArray array, jlong **elements, jint mode);
XJNIAPI void XJNICALL SetLong2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jlong **buf);
XJNIAPI void XJNICALL GetLong2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jlong **buf);
//@}

/** @name Float 2D Array Utility **/
//@{
XJNIAPI jobjectArray XJNICALL NewFloat2DArray(JNIEnv *env, jsize row, jsize col);
XJNIAPI jfloat** XJNICALL GetFloat2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
XJNIAPI void XJNICALL ReleaseFloat2DArrayElements(JNIEnv *env, jobjectArray array, jfloat **elements, jint mode);
XJNIAPI void XJNICALL SetFloat2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jfloat **buf);
XJNIAPI void XJNICALL GetFloat2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jfloat **buf);
//@}

/** @name Double 2D Array Utility **/
//@{
XJNIAPI jobjectArray XJNICALL NewDouble2DArray(JNIEnv *env, jsize row, jsize col);
XJNIAPI jdouble** XJNICALL GetDouble2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
XJNIAPI void XJNICALL ReleaseDouble2DArrayElements(JNIEnv *env, jobjectArray array, jdouble **elements, jint mode);
XJNIAPI void XJNICALL SetDouble2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jdouble **buf);
XJNIAPI void XJNICALL GetDouble2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jdouble **buf);
//@}

/** @name Short 2D Array Utility **/
//@{
XJNIAPI jobjectArray XJNICALL NewShort2DArray(JNIEnv *env, jsize row, jsize col);
XJNIAPI jshort** XJNICALL GetShort2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
XJNIAPI void XJNICALL ReleaseShort2DArrayElements(JNIEnv *env, jobjectArray array, jshort **elements, jint mode);
XJNIAPI void XJNICALL SetShort2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jshort **buf);
XJNIAPI void XJNICALL GetShort2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jshort **buf);
//@}

/** @name Char 2D Array Utility **/
//@{
XJNIAPI jobjectArray XJNICALL NewChar2DArray(JNIEnv *env, jsize row, jsize col);
XJNIAPI jchar** XJNICALL GetChar2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
XJNIAPI void XJNICALL ReleaseChar2DArrayElements(JNIEnv *env, jobjectArray array, jchar **elements, jint mode);
XJNIAPI void XJNICALL SetChar2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jchar **buf);
XJNIAPI void XJNICALL GetChar2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jchar **buf);
//@}

/** @name Boolean 2D Array Utility **/
//@{
XJNIAPI jobjectArray XJNICALL NewBoolean2DArray(JNIEnv *env, jsize row, jsize col);
XJNIAPI jboolean** XJNICALL GetBoolean2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
XJNIAPI void XJNICALL ReleaseBoolean2DArrayElements(JNIEnv *env, jobjectArray array, jboolean **elements, jint mode);
XJNIAPI void XJNICALL SetBoolean2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jboolean **buf);
XJNIAPI void XJNICALL GetBoolean2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jboolean **buf);
//@}

/** @name String UTF 2D Array Utility **/
//@{
XJNIAPI jobjectArray XJNICALL NewStringUTF2DArray(JNIEnv *env,const char ***utf,jsize row,jsize col);
XJNIAPI const char*** XJNICALL GetStringUTF2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
XJNIAPI void XJNICALL ReleaseStringUTF2DArrayChars(JNIEnv *env, jobjectArray array, const char ***elements, jint mode);
XJNIAPI void XJNICALL SetStringUTF2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const char ***buf);
XJNIAPI void XJNICALL GetStringUTF2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, char ***buf);
//@}

/** @name String 2D Array Utility **/
//@{
XJNIAPI jobjectArray XJNICALL NewString2DArray(JNIEnv *env,const jchar ***utf,jsize row,jsize col);
XJNIAPI const jchar*** XJNICALL GetString2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
XJNIAPI void XJNICALL ReleaseString2DArrayChars(JNIEnv *env, jobjectArray array, const jchar ***elements);
XJNIAPI void XJNICALL SetString2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jchar ***buf);
XJNIAPI void XJNICALL GetString2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jchar ***buf);
//@}

/** @} */ // end of JNI_2DArray group

#ifdef __cplusplus
}
#endif

#endif /* __XJNI2D_H__ */
