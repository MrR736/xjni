/**
 * @file xjni_printf.h
 * @brief Extern JNI Printf Utility - printf-style functions for `jchar` strings
 *
 * Provides printf-like functions for `jchar` strings in native code.
 * Supports standard output, file streams, file descriptors, and buffers.
 *
 * @author MrR736
 * @date 2025
 * @copyright GPL-3
 */

#ifndef __XJNI_PRINTF_H__
#define __XJNI_PRINTF_H__

#include <stdio.h>
#include <stdarg.h>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup XJNI_Printf JNI Printf Utilities
 *  @brief Printf-style functions for `jchar` strings
 *  @{
 */

/**
 * @brief Write formatted output to a `jchar` buffer using `va_list`
 * @param s Destination buffer
 * @param format Format string (`jchar*`)
 * @param arg Variable argument list
 * @return Number of characters written (excluding null terminator)
 */
JNIEXPORT int JNICALL vsjprintf(jchar* s, const jchar* format, va_list arg);

/**
 * @brief Write formatted output to a `jchar` buffer
 * @param s Destination buffer
 * @param format Format string (`jchar*`)
 * @param ... Variable arguments
 * @return Number of characters written (excluding null terminator)
 */
JNIEXPORT int JNICALL sjprintf(jchar* s, const jchar* format, ...);

/**
 * @brief Write formatted output to a `jchar` buffer with maximum length using `va_list`
 * @param s Destination buffer
 * @param maxlen Maximum number of characters to write
 * @param format Format string (`jchar*`)
 * @param arg Variable argument list
 * @return Number of characters written (excluding null terminator)
 */
JNIEXPORT int JNICALL vsnjprintf(jchar* s, size_t maxlen, const jchar* format, va_list arg);

/**
 * @brief Write formatted output to a `jchar` buffer with maximum length
 * @param s Destination buffer
 * @param maxlen Maximum number of characters to write
 * @param format Format string (`jchar*`)
 * @param ... Variable arguments
 * @return Number of characters written (excluding null terminator)
 */
JNIEXPORT int JNICALL snjprintf(jchar* s, size_t maxlen, const jchar* format, ...);

/**
 * @brief Write formatted output to a `FILE` stream using `va_list`
 * @param stream Output file stream
 * @param format Format string (`jchar*`)
 * @param arg Variable argument list
 * @return Number of characters written
 */
JNIEXPORT int JNICALL vjfprintf(FILE* stream, const jchar* format, va_list arg);

/**
 * @brief Write formatted output to a `FILE` stream
 * @param stream Output file stream
 * @param format Format string (`jchar*`)
 * @param ... Variable arguments
 * @return Number of characters written
 */
JNIEXPORT int JNICALL jfprintf(FILE* stream, const jchar* format, ...);

/**
 * @brief Write formatted output to a file descriptor using `va_list`
 * @param fd File descriptor
 * @param format Format string (`jchar*`)
 * @param arg Variable argument list
 * @return Number of characters written
 */
JNIEXPORT int JNICALL vjdprintf(int fd, const jchar* format, va_list arg);

/**
 * @brief Write formatted output to a file descriptor
 * @param fd File descriptor
 * @param format Format string (`jchar*`)
 * @param ... Variable arguments
 * @return Number of characters written
 */
JNIEXPORT int JNICALL jdprintf(int fd, const jchar* format, ...);

/**
 * @brief Write formatted output to standard output using `va_list`
 * @param format Format string (`jchar*`)
 * @param arg Variable argument list
 * @return Number of characters written
 */
JNIEXPORT int JNICALL vjprintf(const jchar* format, va_list arg);

/**
 * @brief Write formatted output to standard output
 * @param format Format string (`jchar*`)
 * @param ... Variable arguments
 * @return Number of characters written
 */
JNIEXPORT int JNICALL jprintf(const jchar* format, ...);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_PRINTF_H__ */
