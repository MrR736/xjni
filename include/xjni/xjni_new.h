/**
 * @file xjni_new.h
 * @brief Extern JNI "New" Utility - Create Java objects and arrays from native code
 *
 * Provides functions to create Java arrays and objects from C/C++ native data.
 *
 * @author MrR736
 * @date 2025
 * @copyright GPL-3
 */

#ifndef __XJNI_NEW_H__
#define __XJNI_NEW_H__

#include <jni.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup XJNI_New JNI Object & Array Creation
 *  @brief Functions to create Java arrays and objects from native data
 *  @{
 */

/**
 * @brief Create a new Java object array of the specified class and length
 * @param env JNI environment pointer
 * @param clz Java class for the array elements
 * @param len Number of elements in the array
 * @return Newly created `jobjectArray`
 */
JNIEXPORT jobjectArray JNICALL xjni_NewObjectArray(JNIEnv* env, jclass clz, jsize len);

/**
 * @brief Create a new Java byte array from native `jbyte` data
 * @param env JNI environment pointer
 * @param data Pointer to native byte data
 * @param len Number of bytes
 * @return Newly created `jbyteArray`
 */
JNIEXPORT jbyteArray JNICALL xjni_NewByteArray(JNIEnv* env, const jbyte* data, jsize len);

/**
 * @brief Create a new Java integer array from native `jint` data
 * @param env JNI environment pointer
 * @param data Pointer to native integer data
 * @param len Number of integers
 * @return Newly created `jintArray`
 */
JNIEXPORT jintArray JNICALL xjni_NewIntArray(JNIEnv* env, const jint* data, jsize len);

/**
 * @brief Create a new Java long array from native `jlong` data
 * @param env JNI environment pointer
 * @param data Pointer to native long data
 * @param len Number of longs
 * @return Newly created `jlongArray`
 */
JNIEXPORT jlongArray JNICALL xjni_NewLongArray(JNIEnv* env, const jlong* data, jsize len);

/**
 * @brief Create a new Java float array from native `float` data
 * @param env JNI environment pointer
 * @param data Pointer to native float data
 * @param len Number of floats
 * @return Newly created `jfloatArray`
 */
JNIEXPORT jfloatArray JNICALL xjni_NewFloatArray(JNIEnv* env, const float* data, jsize len);

/**
 * @brief Create a new Java double array from native `double` data
 * @param env JNI environment pointer
 * @param data Pointer to native double data
 * @param len Number of doubles
 * @return Newly created `jdoubleArray`
 */
JNIEXPORT jdoubleArray JNICALL xjni_NewDoubleArray(JNIEnv* env, const double* data, jsize len);

/**
 * @brief Called when the "New" module is loaded
 * @param vm JavaVM pointer
 * @param reserved Reserved pointer (JNI spec)
 * @param ver JNI version
 * @return JNI_OK on success
 */
JNIEXPORT jint JNICALL XJNI_New_OnLoad(JavaVM* vm, void* reserved, jint ver);

/**
 * @brief Called when the "New" module is unloaded
 * @param vm JavaVM pointer
 * @param reserved Reserved pointer (JNI spec)
 * @param ver JNI version
 */
JNIEXPORT void JNICALL XJNI_New_OnUnload(JavaVM* vm, void* reserved, jint ver);

/**
 * @brief Create a new Java object using a constructor signature and `va_list`
 * @param env JNI environment pointer
 * @param className Fully qualified Java class name
 * @param sig Constructor signature
 * @param ap Variable argument list
 * @return Newly created Java object
 */
JNIEXPORT jobject JNICALL NewObjectBuilderV(JNIEnv* env, const char* className, const char* sig, va_list ap);

/**
 * @brief Create a new Java object using a constructor signature and variable arguments
 * @param env JNI environment pointer
 * @param className Fully qualified Java class name
 * @param sig Constructor signature
 * @param ... Variable arguments for constructor
 * @return Newly created Java object
 */
JNIEXPORT jobject JNICALL NewObjectBuilder(JNIEnv* env, const char* className, const char* sig, ...);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_NEW_H__ */
