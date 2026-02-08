/**
 * @file xjni_log.h
 * @brief Extern JNI Log Utility
 *
 * Provides logging utilities for native code interacting with Java via JNI.
 * Supports standard log priorities, optional colors on non-Android platforms,
 * and automatic source file and line tagging.
 *
 * @author MrR736
 * @date 2025
 * @copyright GPL-3
 */

#ifndef __XJNI_LOG_H__
#define __XJNI_LOG_H__

#include <stdarg.h>
#include <jni.h>

/** Default tag used in logging */
#define XJNI_DEFAULT_TAG "XJNI"

/** @enum xjni_LogPriority
 *  @brief Logging priority levels
 */
typedef enum xjni_LogPriority {
  XJNI_LOG_UNKNOWN = 0,   /**< Internal use only */
  XJNI_LOG_DEFAULT,       /**< Default priority, internal use only */
  XJNI_LOG_VERBOSE,       /**< Verbose logging (usually disabled in release) */
  XJNI_LOG_DEBUG,         /**< Debug logging (usually disabled in release) */
  XJNI_LOG_INFO,          /**< Informational logging */
  XJNI_LOG_WARN,          /**< Warning logging, recoverable failures */
  XJNI_LOG_ERROR,         /**< Error logging, unrecoverable failures */
  XJNI_LOG_FATAL,         /**< Fatal logging, for abort scenarios */
  XJNI_LOG_SILENT,        /**< Internal use only */
} xjni_LogPriority;

/** Convenience macros for printing logs with automatic file and line info */
#define xjni_log_vprint(prio,tag,fmt,ap) \
    xjni_log_vcprint(prio,__LINE__,__FILE__,tag,fmt,ap)

#define xjni_log_print(prio,tag,fmt,...) \
    xjni_log_cprint(prio,__LINE__,__FILE__,tag,fmt,##__VA_ARGS__)

/** Platform-specific macros for log printing */
#ifdef __ANDROID__
#define xjni_log_vccprint xjni_log_vcprint
#define xjni_log_ccprint xjni_log_cprint
#define xjni_log_vprintc xjni_log_vprint
#define xjni_log_printc xjni_log_print
#else
#define xjni_log_vprintc(prio,tag,fmt,ap) xjni_log_vccprint(prio,__LINE__,__FILE__,tag,fmt,ap)
#define xjni_log_printc(prio,tag,fmt,...) xjni_log_ccprint(prio,__LINE__,__FILE__,tag,fmt,##__VA_ARGS__)
#endif

/** Standard log level macros */
#define XJNI_LOGE(tag,fmt,...) xjni_log_print(XJNI_LOG_ERROR,tag,fmt,##__VA_ARGS__)
#define XJNI_LOGI(tag,fmt,...) xjni_log_print(XJNI_LOG_INFO,tag,fmt,##__VA_ARGS__)
#define XJNI_LOGV(tag,fmt,...) xjni_log_print(XJNI_LOG_VERBOSE,tag,fmt,##__VA_ARGS__)
#define XJNI_LOGD(tag,fmt,...) xjni_log_print(XJNI_LOG_DEBUG,tag,fmt,##__VA_ARGS__)
#define XJNI_LOGW(tag,fmt,...) xjni_log_print(XJNI_LOG_WARN, tag,fmt,##__VA_ARGS__)
#define XJNI_LOGF(tag,fmt,...) xjni_log_print(XJNI_LOG_FATAL,tag,fmt,##__VA_ARGS__)

/** Colored or console variants (non-Android) */
#define XJNI_LOGEC(tag,fmt,...) xjni_log_printc(XJNI_LOG_ERROR,tag,fmt,##__VA_ARGS__)
#define XJNI_LOGIC(tag,fmt,...) xjni_log_printc(XJNI_LOG_INFO,tag,fmt,##__VA_ARGS__)
#define XJNI_LOGVC(tag,fmt,...) xjni_log_printc(XJNI_LOG_VERBOSE,tag,fmt,##__VA_ARGS__)
#define XJNI_LOGDC(tag,fmt,...) xjni_log_printc(XJNI_LOG_DEBUG,tag,fmt,##__VA_ARGS__)
#define XJNI_LOGWC(tag,fmt,...) xjni_log_printc(XJNI_LOG_WARN,tag,fmt,##__VA_ARGS__)
#define XJNI_LOGFC(tag,fmt,...) xjni_log_printc(XJNI_LOG_FATAL,tag,fmt,##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ANDROID__
/** Log to console with colors (non-Android) */
JNIEXPORT int JNICALL xjni_log_vccprint(int prio,int line,const char* file,const char* tag,const char* fmt,va_list ap);
JNIEXPORT int JNICALL xjni_log_ccprint(int prio,int line,const char* file,const char* tag,const char* fmt,...);
#endif

/** Log to console with optional colors */
JNIEXPORT int JNICALL xjni_log_vcprint(int prio,int line,const char* file,const char* tag,const char* fmt,va_list ap);
JNIEXPORT int JNICALL xjni_log_cprint(int prio,int line,const char* file,const char* tag,const char* fmt,...);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_LOG_H__ */
