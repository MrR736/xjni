/**
 * @file xjni2d.h
 * @brief Extern JNI 2D Array Utility
 *
 * Provides creation, access, release, and region functions for
 * Java 2D arrays of all primitive types and strings.
 *
 * @author MrR736
 * @date 2026
 * @copyright GPL-3
 */

#ifndef __XJNI2D_H__
#define __XJNI2D_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup JNI_2DArray Java 2D Array Utilities
 *  Functions to create, access, and manipulate Java 2D arrays from native C/C++.
 *  @{
 */

/** @name Byte 2D Array Utility **/
//@{
JNIEXPORT jobjectArray JNICALL NewByte2DArray(JNIEnv *env, jsize row, jsize col);
JNIEXPORT jbyte** JNICALL GetByte2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
JNIEXPORT void JNICALL ReleaseByte2DArrayElements(JNIEnv *env, jobjectArray array, jbyte **elements, jint mode);
JNIEXPORT void JNICALL SetByte2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jbyte **buf);
JNIEXPORT void JNICALL GetByte2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jbyte **buf);
//@}

/** @name Int 2D Array Utility **/
//@{
JNIEXPORT jobjectArray JNICALL NewInt2DArray(JNIEnv *env, jsize row, jsize col);
JNIEXPORT jint** JNICALL GetInt2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
JNIEXPORT void JNICALL ReleaseInt2DArrayElements(JNIEnv *env, jobjectArray array, jint **elements, jint mode);
JNIEXPORT void JNICALL SetInt2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jint **buf);
JNIEXPORT void JNICALL GetInt2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jint **buf);
//@}

/** @name Long 2D Array Utility **/
//@{
JNIEXPORT jobjectArray JNICALL NewLong2DArray(JNIEnv *env, jsize row, jsize col);
JNIEXPORT jlong** JNICALL GetLong2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
JNIEXPORT void JNICALL ReleaseLong2DArrayElements(JNIEnv *env, jobjectArray array, jlong **elements, jint mode);
JNIEXPORT void JNICALL SetLong2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jlong **buf);
JNIEXPORT void JNICALL GetLong2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jlong **buf);
//@}

/** @name Float 2D Array Utility **/
//@{
JNIEXPORT jobjectArray JNICALL NewFloat2DArray(JNIEnv *env, jsize row, jsize col);
JNIEXPORT jfloat** JNICALL GetFloat2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
JNIEXPORT void JNICALL ReleaseFloat2DArrayElements(JNIEnv *env, jobjectArray array, jfloat **elements, jint mode);
JNIEXPORT void JNICALL SetFloat2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jfloat **buf);
JNIEXPORT void JNICALL GetFloat2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jfloat **buf);
//@}

/** @name Double 2D Array Utility **/
//@{
JNIEXPORT jobjectArray JNICALL NewDouble2DArray(JNIEnv *env, jsize row, jsize col);
JNIEXPORT jdouble** JNICALL GetDouble2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
JNIEXPORT void JNICALL ReleaseDouble2DArrayElements(JNIEnv *env, jobjectArray array, jdouble **elements, jint mode);
JNIEXPORT void JNICALL SetDouble2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jdouble **buf);
JNIEXPORT void JNICALL GetDouble2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jdouble **buf);
//@}

/** @name Short 2D Array Utility **/
//@{
JNIEXPORT jobjectArray JNICALL NewShort2DArray(JNIEnv *env, jsize row, jsize col);
JNIEXPORT jshort** JNICALL GetShort2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
JNIEXPORT void JNICALL ReleaseShort2DArrayElements(JNIEnv *env, jobjectArray array, jshort **elements, jint mode);
JNIEXPORT void JNICALL SetShort2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jshort **buf);
JNIEXPORT void JNICALL GetShort2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jshort **buf);
//@}

/** @name Char 2D Array Utility **/
//@{
JNIEXPORT jobjectArray JNICALL NewChar2DArray(JNIEnv *env, jsize row, jsize col);
JNIEXPORT jchar** JNICALL GetChar2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
JNIEXPORT void JNICALL ReleaseChar2DArrayElements(JNIEnv *env, jobjectArray array, jchar **elements, jint mode);
JNIEXPORT void JNICALL SetChar2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jchar **buf);
JNIEXPORT void JNICALL GetChar2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jchar **buf);
//@}

/** @name Boolean 2D Array Utility **/
//@{
JNIEXPORT jobjectArray JNICALL NewBoolean2DArray(JNIEnv *env, jsize row, jsize col);
JNIEXPORT jboolean** JNICALL GetBoolean2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
JNIEXPORT void JNICALL ReleaseBoolean2DArrayElements(JNIEnv *env, jobjectArray array, jboolean **elements, jint mode);
JNIEXPORT void JNICALL SetBoolean2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jboolean **buf);
JNIEXPORT void JNICALL GetBoolean2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jboolean **buf);
//@}

/** @name String UTF 2D Array Utility **/
//@{
JNIEXPORT jobjectArray JNICALL NewStringUTF2DArray(JNIEnv *env,const char ***utf,jsize row,jsize col);
JNIEXPORT const char*** JNICALL GetStringUTF2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
JNIEXPORT void JNICALL ReleaseStringUTF2DArrayChars(JNIEnv *env, jobjectArray array, const char ***elements, jint mode);
JNIEXPORT void JNICALL SetStringUTF2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const char ***buf);
JNIEXPORT void JNICALL GetStringUTF2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, char ***buf);
//@}

/** @name String 2D Array Utility **/
//@{
JNIEXPORT jobjectArray JNICALL NewString2DArray(JNIEnv *env,const jchar ***utf,jsize row,jsize col);
JNIEXPORT const jchar*** JNICALL GetString2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);
JNIEXPORT void JNICALL ReleaseString2DArrayChars(JNIEnv *env, jobjectArray array, const jchar ***elements);
JNIEXPORT void JNICALL SetString2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jchar ***buf);
JNIEXPORT void JNICALL GetString2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jchar ***buf);
//@}

/** @} */ // end of JNI_2DArray group

#ifdef __cplusplus
}
#endif

#endif /* __XJNI2D_H__ */
