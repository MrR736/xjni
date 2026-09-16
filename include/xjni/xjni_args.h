/**
 * @file xjni_args.h
 * @brief Extern JNI Args Utility - utility functions for creating and manipulating jobjectArray arguments
 *
 * Provides helper functions for constructing and modifying Java argument arrays (`jobjectArray`)
 * from native code, supporting primitive and object types.
 *
 * @author MrR736
 * @date 2026
 * @copyright MIT
 */

#ifndef __XJNI_ARGS_H__
#define __XJNI_ARGS_H__

#include <stdarg.h>
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
JNIEXPORT void JNICALL JArgsAppendByte(JNIEnv *env, jargs_t args, jbyte obj);
JNIEXPORT void JNICALL JArgsAppendShort(JNIEnv *env, jargs_t args, jshort obj);
/** @} */

/** @defgroup XJNI_Args_PushBack PushBack Elements
 *  @brief Functions to append elements to a Java argument array
 *  @{
 */
#define JArgsPushBackObject JArgsAppendObject
#define JArgsPushBackString JArgsAppendString
#define JArgsPushBackStringUTF JArgsAppendStringUTF
#define JArgsPushBackChar JArgsAppendChar
#define JArgsPushBackBoolean JArgsAppendBoolean
#define JArgsPushBackInt JArgsAppendInt
#define JArgsPushBackLong JArgsAppendLong
#define JArgsPushBackFloat JArgsAppendFloat
#define JArgsPushBackDouble JArgsAppendDouble
#define JArgsPushBackByte JArgsAppendByte
#define JArgsPushBackShort JArgsAppendShort
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
JNIEXPORT void JNICALL JArgsInsertByte(JNIEnv *env, jargs_t args, jbyte obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertShort(JNIEnv *env, jargs_t args, jshort obj, jsize index);
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
JNIEXPORT void JNICALL JArgsReplaceByte(JNIEnv *env, jargs_t args, jbyte obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceShort(JNIEnv *env, jargs_t args, jshort obj, jsize index);
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
JNIEXPORT jbyte JNICALL GetJArgsByte(JNIEnv *env, jargs_t args, jsize index);
JNIEXPORT jshort JNICALL GetJArgsShort(JNIEnv *env, jargs_t args, jsize index);
/** @} */

/** @defgroup XJNI_Args_Start Start Elements
 *  @brief Functions to create and populate Java argument arrays from C/varargs.
 *
 *  These functions allow building a `jargs_t` array from a C-style
 *  variable argument list, based on a JNI-style signature string.
 *
 *  Example usage:
 *  @code
 *      jstring s = (*env)->NewStringUTF(env, "Hello");
 *      jargs_t args = JArgsStart(env, 3, "Ljava/lang/String;IF", s, 42, 3.14f);
 *  @endcode
 *  - `"L…;"` types are objects; `"Ljava/lang/String;"` expects a `jstring`.
 *  - Primitives are promoted according to C default promotion rules:
 *      - `float` → `double`
 *      - `char`, `short`, `boolean`, `byte` → `int`
 *  - Other object types (`L<classname>;`) expect `jobject`.
 *
 *  @{
 */

/**
 * @brief Builds a Java argument array from a signature string and va_list.
 *
 * Parses a JNI-style method signature and extracts corresponding
 * arguments from the provided variable argument list.
 *
 * Supported signature types:
 *  - L<class>;  Object type (e.g., Ljava/lang/String;)
 *  - I          jint
 *  - J          jlong
 *  - Z          jboolean
 *  - F          jfloat
 *  - D          jdouble
 *  - C          jchar
 *  - S          jshort
 *  - B          jbyte
 *
 * Object types:
 *  - java/lang/String → handled via JArgsAppendString()
 *  - Other objects    → validated via IsSameObjectChars()
 *
 * @param env   Pointer to the JNI environment.
 * @param index Number of expected arguments.
 * @param sig   JNI method signature string.
 * @param ap    Variable argument list containing argument values.
 *
 * @return Constructed jargs_t containing appended arguments.
 *
 * @note The signature must follow JNI descriptor format.
 * @note Caller is responsible for proper va_list handling.
 */
JNIEXPORT jargs_t JNICALL JArgsStartV(JNIEnv *env, jsize index, const char* sig, va_list ap);

/**
 * @brief Create a Java argument array from C variable arguments.
 * @param env JNI environment pointer
 * @param index Number of elements in the argument array
 * @param sig JNI-style signature string describing argument types
 * @param ... Values corresponding to the signature
 * @return A `jargs_t` array with boxed Java objects
 */
JNIEXPORT jargs_t JNICALL JArgsStart(JNIEnv *env, jsize index, const char* sig, ...);

/** @} */

/** @{ */
/**
 * @brief Removes the last element from a Java argument array.
 *
 * Deletes the element at the current last index of the supplied
 * Java argument array.
 *
 * @param env  Pointer to the JNI environment.
 * @param args Java argument array to modify.
 *
 * @note If the array is empty, no element is removed.
 * @note The argument array itself is not resized by this function.
 */
JNIEXPORT void JNICALL JArgsPopBack(JNIEnv *env, jargs_t args);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_ARGS_H__ */
