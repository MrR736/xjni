/**
 * @file xjni_va_list.h
 * @brief Extern JNI VA LIST Utility - variable argument printing helpers for JNI
 *
 * Provides utility functions to print formatted strings using Java strings (`jstring`)
 * and argument arrays (`jargs_t`) in native code, supporting standard output, file streams,
 * and file descriptors.
 *
 * @author MrR736
 * @date 2026
 * @copyright GPL-3
 */

#ifndef __XJNI_VA_LIST_H__
#define __XJNI_VA_LIST_H__

#include <stdio.h>
#include <jni.h>
#include <xjni_args.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup XJNI_VA_Print VA List Printing
 *  @brief Functions to print formatted output using `jargs_t` arrays
 *  @{
 */

/**
 * @brief Print formatted string to a buffer using a Java format string
 * @param env JNI environment
 * @param s Destination buffer
 * @param maxlen Maximum length of buffer
 * @param format Java string format
 * @param args Argument array (`jargs_t`)
 */
JNIEXPORT void JNICALL JSnPrintf(JNIEnv *env, char* s, size_t maxlen, jstring format, jargs_t args);

/**
 * @brief Print formatted string to a buffer using UTF-8 C format string
 * @param env JNI environment
 * @param s Destination buffer
 * @param maxlen Maximum length of buffer
 * @param format UTF-8 format string
 * @param args Argument array (`jargs_t`)
 */
JNIEXPORT void JNICALL JSnPrintfUTF(JNIEnv *env, char* s, size_t maxlen, const char* format, jargs_t args);

/**
 * @brief Print formatted string to a buffer using a Java format string (no length limit)
 */
JNIEXPORT void JNICALL JSPrintf(JNIEnv *env, char* s, jstring format, jargs_t args);

/**
 * @brief Print formatted string to a buffer using UTF-8 C format string (no length limit)
 */
JNIEXPORT void JNICALL JSPrintfUTF(JNIEnv *env, char* s, const char* format, jargs_t args);

/**
 * @brief Print formatted string to a FILE stream using a Java format string
 */
JNIEXPORT void JNICALL JFPrintf(JNIEnv *env, FILE* fp, jstring format, jargs_t args);

/**
 * @brief Print formatted string to a FILE stream using UTF-8 C format string
 */
JNIEXPORT void JNICALL JFPrintfUTF(JNIEnv *env, FILE* fp, const char* format, jargs_t args);

/**
 * @brief Print formatted string to a file descriptor using a Java format string
 */
JNIEXPORT void JNICALL JDPrintf(JNIEnv *env, int fd, jstring format, jargs_t args);

/**
 * @brief Print formatted string to a file descriptor using UTF-8 C format string
 */
JNIEXPORT void JNICALL JDPrintfUTF(JNIEnv *env, int fd, const char* format, jargs_t args);

/**
 * @brief Print formatted string to standard output using a Java format string
 */
JNIEXPORT void JNICALL JPrintf(JNIEnv *env, jstring format, jargs_t args);

/**
 * @brief Print formatted string to standard output using UTF-8 C format string
 */
JNIEXPORT void JNICALL JPrintfUTF(JNIEnv *env, const char* format, jargs_t args);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_VA_LIST_H__ */
