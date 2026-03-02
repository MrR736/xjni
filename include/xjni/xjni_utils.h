/**
 * @file xjni_utils.h
 * @brief JNI utility functions for XJNI runtime.
 *
 * Provides convenience functions for accessing fields by name,
 * and lifecycle hooks for JNI_OnLoad / JNI_OnUnload.
 *
 * @author MrR736
 * @date 2026
 * @license GPL-3.0
 */

#ifndef __XJNI_UTILS_H__
#define __XJNI_UTILS_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup XJNI_Utils XJNI Utilities
 * @brief Core JNI helper functions for runtime management.
 *
 * Provides functions to:
 *  - Retrieve field IDs using class caching.
 *  - Initialize and clean up runtime resources on library load/unload.
 *
 * @note All functions are thread-safe where applicable, except during JVM shutdown.
 *
 * @{
 */

/**
 * @brief Checks whether a Java object is an instance of a given class.
 *
 * This function finds the class specified by the fully-qualified
 * JNI class name and verifies whether the provided object is
 * an instance of that class.
 *
 * The class name must use JNI slash notation:
 * Example: "java/lang/String"
 *
 * @param env        Pointer to the JNI environment.
 * @param className  Fully-qualified class name in JNI format
 *                   (e.g., "java/lang/String").
 * @param obj2       Java object to test.
 *
 * @return JNI_TRUE  if obj2 is an instance of the specified class.
 * @return JNI_FALSE if obj2 is NULL, className is NULL,
 *                   class is not found, or object is not an instance.
 */
JNIEXPORT jboolean JNICALL IsSameObjectChars(JNIEnv *env, const char* className, jobject obj2);

/**
 * @brief Retrieve a field ID by class name, field name, and signature.
 *
 * Uses the XJNI class cache to minimize repeated FindClass calls.
 *
 * @param env JNI environment pointer.
 * @param className Fully-qualified Java class name (e.g., "java/lang/String").
 * @param fieldName Name of the field to retrieve.
 * @param sig JNI field signature (e.g., "Ljava/lang/String;").
 *
 * @return jfieldID on success, or NULL if the class or field was not found.
 *
 * @note The returned jfieldID is JVM-managed; do not call DeleteGlobalRef.
 */
JNIEXPORT jfieldID JNICALL GetFieldIDByName(JNIEnv* env,const char* className,const char* fieldName,const char* sig);

/**
 * @brief Retrieve a field ID from a given object by field name and signature.
 *
 * @param env JNI environment pointer.
 * @param obj Java object from which to get the class.
 * @param fieldName Name of the field.
 * @param sig JNI signature of the field.
 *
 * @return jfieldID on success, NULL if class or field not found.
 *
 * @note Returned jfieldID is JVM-managed; do not delete it.
 *
 * @see GetFieldIDByName
 * @see GetObjectNameChars
 */
JNIEXPORT jfieldID JNICALL GetFieldIDByObject(JNIEnv* env,jobject obj,const char* fieldName,const char* sig);

/**
 * @brief Retrieves the fully-qualified Java class name of an object.
 *
 * This function obtains the runtime class of the given Java object
 * and calls java.lang.Class.getName() to retrieve its fully-qualified
 * class name (e.g. "java.lang.String").
 *
 * @param env Pointer to the JNI environment.
 * @param obj Java object instance whose class name will be retrieved.
 *
 * @return A jstring containing the fully-qualified class name.
 *
 * @note The returned jstring is a local reference and is valid only
 *       for the duration of the current JNI call unless promoted
 *       to a global reference.
 *
 * @warning The caller must ensure that `obj` is not NULL.
 */
JNIEXPORT jstring JNICALL GetObjectName(JNIEnv* env, jobject obj);

/**
 * @brief Retrieves the fully-qualified Java class name of an object as a C string.
 *
 * This function calls GetObjectName() internally and converts the
 * returned jstring into a UTF-8 encoded C string.
 *
 * @param env Pointer to the JNI environment.
 * @param obj Java object instance whose class name will be retrieved.
 *
 * @return A newly allocated null-terminated UTF-8 C string containing
 *         the fully-qualified class name.
 *
 * @note The returned pointer must be freed by the caller using free().
 *
 * @warning Returning raw pointers across JNI boundaries is generally
 *          discouraged. Prefer returning jstring when possible.
 *
 * @see GetObjectName
 */
JNIEXPORT char* JNICALL GetObjectNameChars(JNIEnv* env, jobject obj);

/**
 * @brief Called during JNI_OnLoad to initialize the XJNI runtime.
 *
 * Initializes the class cache and prepares runtime resources.
 *
 * @param vm Pointer to the JavaVM.
 * @param reserved Reserved pointer (unused).
 * @param ver JNI version requested.
 *
 * @return JNI version on success, or JNI_ERR on failure.
 */
JNIEXPORT jint JNICALL XJNI_Utily_OnLoad(JavaVM* vm,void* reserved,jint ver);

/**
 * @brief Called during JNI_OnUnload to clean up the XJNI runtime.
 *
 * Clears the class cache and releases global references.
 * Attaches the current thread temporarily if necessary.
 *
 * @param vm Pointer to the JavaVM.
 * @param reserved Reserved pointer (unused).
 * @param ver JNI version.
 */
JNIEXPORT void JNICALL XJNI_Utily_OnUnload(JavaVM* vm,void* reserved,jint ver);

/** @} */ /* End of XJNI_Utils group */

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_UTILS_H__ */
