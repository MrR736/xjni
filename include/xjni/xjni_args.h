/**
 * @file xjni_args.h
 * @brief Extern JNI Args Utility - utility functions for creating and manipulating jobjectArray arguments
 *
 * Provides helper functions for constructing and modifying Java argument arrays (`jobjectArray`)
 * from native code, supporting primitive and object types.
 *
 * @author MrR736
 * @date 2026
 * @copyright GPL-3
 */

#ifndef __XJNI_ARGS_H__
#define __XJNI_ARGS_H__

#include <jni.h>

/** @typedef jargs_t
 *  @brief Type alias for a Java argument array
 */
typedef jobjectArray jargs_t;

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup XJNI_Args Creation
 *  @brief Functions for creating Java argument arrays
 *  @{
 */

/**
 * @brief Create a new Java argument array
 * @param env JNI environment pointer
 * @param index Initial array length
 * @param cls Java class of array elements
 * @param init Initial element (can be NULL)
 * @return Newly created jobjectArray
 */
JNIEXPORT jargs_t JNICALL NewJArgs(JNIEnv *env, jsize index, jclass cls, jobject init);
/** @} */

/** @defgroup XJNI_Args_Append Append Elements
 *  @brief Functions to append elements to a Java argument array
 *  @{
 */
JNIEXPORT void JNICALL JArgsAppendObject(JNIEnv *env, jargs_t args, jobject obj);
JNIEXPORT void JNICALL JArgsAppendString(JNIEnv *env, jargs_t args, jstring obj);
JNIEXPORT void JNICALL JArgsAppendStringUTF(JNIEnv *env, jargs_t args, const char* fmt);
JNIEXPORT void JNICALL JArgsAppendChar(JNIEnv *env, jargs_t args, jchar obj);
JNIEXPORT void JNICALL JArgsAppendBoolean(JNIEnv *env, jargs_t args, jboolean obj);
JNIEXPORT void JNICALL JArgsAppendInt(JNIEnv *env, jargs_t args, jint obj);
JNIEXPORT void JNICALL JArgsAppendLong(JNIEnv *env, jargs_t args, jlong obj);
JNIEXPORT void JNICALL JArgsAppendFloat(JNIEnv *env, jargs_t args, jfloat obj);
JNIEXPORT void JNICALL JArgsAppendDouble(JNIEnv *env, jargs_t args, jdouble obj);
/** @} */

/** @defgroup XJNI_Args_Insert Insert Elements
 *  @brief Functions to insert elements at a specific index
 *  @{
 */
JNIEXPORT void JNICALL JArgsInsertObject(JNIEnv *env, jargs_t args, jobject obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertString(JNIEnv *env, jargs_t args, jstring obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertStringUTF(JNIEnv *env, jargs_t args, const char* fmt, jsize index);
JNIEXPORT void JNICALL JArgsInsertChar(JNIEnv *env, jargs_t args, jchar obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertBoolean(JNIEnv *env, jargs_t args, jboolean obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertInt(JNIEnv *env, jargs_t args, jint obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertLong(JNIEnv *env, jargs_t args, jlong obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertFloat(JNIEnv *env, jargs_t args, jfloat obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertDouble(JNIEnv *env, jargs_t args, jdouble obj, jsize index);
/** @} */

/** @defgroup XJNI_Args_Replace Replace Elements
 *  @brief Functions to replace elements at a specific index
 *  @{
 */
JNIEXPORT void JNICALL JArgsReplaceObject(JNIEnv *env, jargs_t args, jobject obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceString(JNIEnv *env, jargs_t args, jstring obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceStringUTF(JNIEnv *env, jargs_t args, const char* fmt, jsize index);
JNIEXPORT void JNICALL JArgsReplaceChar(JNIEnv *env, jargs_t args, jchar obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceBoolean(JNIEnv *env, jargs_t args, jboolean obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceInt(JNIEnv *env, jargs_t args, jint obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceLong(JNIEnv *env, jargs_t args, jlong obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceFloat(JNIEnv *env, jargs_t args, jfloat obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceDouble(JNIEnv *env, jargs_t args, jdouble obj, jsize index);
/** @} */

/** @defgroup XJNI_Args_Delete Delete Elements
 *  @brief Remove elements from Java argument array
 *  @{
 */
JNIEXPORT void JNICALL JArgsDelete(JNIEnv *env, jargs_t args, jsize index);
/** @} */

/** @defgroup XJNI_Args_Get Retrieve Elements
 *  @brief Functions to get elements from Java argument array
 *  @{
 */
JNIEXPORT jobject JNICALL GetJArgs(JNIEnv *env, jargs_t args, jsize index);
JNIEXPORT jstring JNICALL GetJArgsString(JNIEnv *env, jargs_t args, jsize index);

/**
 * @brief Get UTF-8 string from Java argument
 * @param env JNI environment
 * @param args Argument array
 * @param index Index of element
 * @return Heap-allocated UTF-8 string (caller MUST free)
 */
JNIEXPORT char* JNICALL GetJArgsStringUTF(JNIEnv *env, jargs_t args, jsize index);

JNIEXPORT jchar JNICALL GetJArgsChar(JNIEnv *env, jargs_t args, jsize index);
JNIEXPORT jboolean JNICALL GetJArgsBoolean(JNIEnv *env, jargs_t args, jsize index);
JNIEXPORT jlong JNICALL GetJArgsLong(JNIEnv *env, jargs_t args, jsize index);
JNIEXPORT jfloat JNICALL GetJArgsFloat(JNIEnv *env, jargs_t args, jsize index);
JNIEXPORT jdouble JNICALL GetJArgsDouble(JNIEnv *env, jargs_t args, jsize index);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_ARGS_H__ */
