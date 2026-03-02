/**
 * @file xjni.h
 * @brief Extern JNI Utility - comprehensive utilities for JNI string, memory, and exception handling
 *
 * This file provides helper functions for working with JNI, including:
 * - jchar / UTF-8 string conversions
 * - memory and string utilities
 * - exception throwing utilities
 * - JNI lifecycle hooks (OnLoad / OnUnload)
 *
 * @author MrR736
 * @date 2026
 * @copyright GPL-3
 */

#ifndef __XJNI_H__
#define __XJNI_H__

#include <stdio.h>
#include <stddef.h>
#include <jni.h>

#include <xjni_new.h>
#include <xjni_classcache.h>
#include <xjni_hashmap.h>
#include <xjni_printf.h>
#include <xjni_arrayfield.h>
#include <xjni_string.h>
#include <xjni_stringarray.h>
#include <xjni_stringbuilder.h>
#include <xjni_stringbuffer.h>
#include <xjni_stringreader.h>
#include <xjni_stringwriter.h>
#include <xjni_va_list.h>
#include <xjni_struct.h>
#include <xjni_thread.h>
#include <xjni_utils.h>
#include <xjni_log.h>
#include <xjni2d.h>

/** @defgroup XJNI_VERSION Version Macros
 *  @brief Version information for XJNI
 *  @{
 */
#ifndef _XJNI_VERSION_MAJOR
#define _XJNI_VERSION_MAJOR	1
#endif
#ifndef _XJNI_VERSION_MINOR
#define _XJNI_VERSION_MINOR	1
#endif
#ifndef _XJNI_VERSION_PATCH
#define _XJNI_VERSION_PATCH	0
#endif

#define _XJNI_VERSION		((_XJNI_VERSION_MAJOR * 1000) + (_XJNI_VERSION_MINOR * 100) + _XJNI_VERSION_PATCH)
/** @} */

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup XJNI_Version Version & Conversion Utilities
 *  @brief Functions for version info and string/jchar conversions
 *  @{
 */

/**
 * @brief Get XJNI library version as string
 * @return Constant version string
 */
JNIEXPORT const char* JNICALL xjni_version(void);
/** @} */

/** @defgroup XJNI_Exception Exception Utilities
 *  @brief Functions for throwing Java exceptions from native code
 *  @{
 */
JNIEXPORT void JNICALL FatalErrorV(JNIEnv *env, const char *msg, va_list __arg);
JNIEXPORT void JNICALL FatalErrorF(JNIEnv *env, const char *msg, ...);

JNIEXPORT jint JNICALL ThrowNewV(JNIEnv *env, jclass clazz, const char *msg, va_list __arg);
JNIEXPORT jint JNICALL ThrowNewF(JNIEnv *env, jclass clazz, const char *msg, ...);

JNIEXPORT void JNICALL throwJava(JNIEnv *env,const char* tag,const char* msg, const char* cls_name, jclass* cache);
JNIEXPORT void JNICALL throwJavaV(JNIEnv *env,const char* tag,const char* clsName,jclass* cache,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwJavaF(JNIEnv *env,const char* tag,const char* cls_name,jclass* cache,const char* msg,...);
/** @} */

/** @name Specific Java Exception Utilities */
/** @{ */
JNIEXPORT void JNICALL throwIOException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwCharConversionException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwEOFException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwFileNotFoundException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwInterruptedIOException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwInvalidClassException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwInvalidObjectException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwNotActiveException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwNotSerializableException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwOptionalDataException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwStreamCorruptedException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwSyncFailedException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwUTFDataFormatException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwUnsupportedEncodingException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwWriteAbortedException(JNIEnv *env, const char* tag, const char* msg);
JNIEXPORT void JNICALL throwOutOfMemoryError(JNIEnv *env, const char* tag, const char* msg);
/** @} */

/** @name Variadic Exception Variants */
/** @{ */
JNIEXPORT void JNICALL throwIOExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwCharConversionExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwEOFExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwFileNotFoundExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwInterruptedIOExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwInvalidClassExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwInvalidObjectExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwNotActiveExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwNotSerializableExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwOptionalDataExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwStreamCorruptedExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwSyncFailedExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwUTFDataFormatExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwUnsupportedEncodingExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwWriteAbortedExceptionV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
JNIEXPORT void JNICALL throwOutOfMemoryErrorV(JNIEnv *env, const char* tag, const char* msg, va_list ap);
/** @} */

/** @name Exception Variants (F - formatted) */
/** @{ */
JNIEXPORT void JNICALL throwIOExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
JNIEXPORT void JNICALL throwCharConversionExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
JNIEXPORT void JNICALL throwEOFExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
JNIEXPORT void JNICALL throwFileNotFoundExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
JNIEXPORT void JNICALL throwInterruptedIOExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
JNIEXPORT void JNICALL throwInvalidClassExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
JNIEXPORT void JNICALL throwInvalidObjectExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
JNIEXPORT void JNICALL throwNotActiveExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
JNIEXPORT void JNICALL throwNotSerializableExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
JNIEXPORT void JNICALL throwOptionalDataExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
JNIEXPORT void JNICALL throwStreamCorruptedExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
JNIEXPORT void JNICALL throwSyncFailedExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
JNIEXPORT void JNICALL throwUTFDataFormatExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
JNIEXPORT void JNICALL throwUnsupportedEncodingExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
JNIEXPORT void JNICALL throwWriteAbortedExceptionF(JNIEnv *env,const char* tag,const char* msg,...);
/** @} */

/** @defgroup XJNI_Lifecycle JNI Lifecycle Hooks
 *  @brief Hooks for loading/unloading native library
 *  @{
 */
JNIEXPORT jint JNICALL XJNI_OnLoad(JavaVM* vm, void* reserved, jint ver);
JNIEXPORT void JNICALL XJNI_OnUnload(JavaVM* vm, void* reserved, jint ver);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_H__ */
