/**
 * xjni_log.h: Extern JNI Log Utility
 *
 * Copyright (C) 2025 MrR736 <MrR736@users.github.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * The complete text of the GNU General Public License
 * can be found in /usr/share/common-licenses/GPL-3 file.
 */

#ifndef __XJNI_LOG_H__
#define __XJNI_LOG_H__

#include <stdarg.h>

#define XJNI_DEFAULT_TAG "XJNI"

typedef enum xjni_LogPriority {
  /** For internal use only.  */
  XJNI_LOG_UNKNOWN = 0,
  /** The default priority, for internal use only.  */
  XJNI_LOG_DEFAULT,
  /** Verbose logging. Should typically be disabled for a release apk. */
  XJNI_LOG_VERBOSE,
  /** Debug logging. Should typically be disabled for a release apk. */
  XJNI_LOG_DEBUG,
  /** Informational logging. Should typically be disabled for a release apk. */
  XJNI_LOG_INFO,
  /** Warning logging. For use with recoverable failures. */
  XJNI_LOG_WARN,
  /** Error logging. For use with unrecoverable failures. */
  XJNI_LOG_ERROR,
  /** Fatal logging. For use when aborting. */
  XJNI_LOG_FATAL,
  /** For internal use only.  */
  XJNI_LOG_SILENT,
} xjni_LogPriority;


#define xjni_log_vprint(prio,tag,fmt,ap) xjni_log_vcprint(prio,__LINE__,__FILE__,tag,fmt,ap)
#define xjni_log_print(prio,tag,fmt,...) xjni_log_cprint(prio,__LINE__,__FILE__,tag,fmt,##__VA_ARGS__)

#ifdef __ANDROID__
#define xjni_log_vccprint xjni_log_vcprint
#define xjni_log_ccprint xjni_log_cprint
#define xjni_log_vprintc xjni_log_vprint
#define xjni_log_printc xjni_log_print
#else
#define xjni_log_vprintc(prio,tag,fmt,ap) xjni_log_vccprint(prio,__LINE__,__FILE__,tag,fmt,ap)
#define xjni_log_printc(prio,tag,fmt,...) xjni_log_ccprint(prio,__LINE__,__FILE__,tag,fmt,##__VA_ARGS__)
#endif

/*
 * Example:
 *   XJNI_LOGI("MyTag","Hello,%s!","World");
 *   XJNI_LOGE("MyTag","An error occurred: %d",errCode);
 */
#define XJNI_LOGE(tag,fmt,...) xjni_log_print(XJNI_LOG_ERROR,tag,fmt,##__VA_ARGS__)
#define XJNI_LOGI(tag,fmt,...) xjni_log_print(XJNI_LOG_INFO,tag,fmt,##__VA_ARGS__)
#define XJNI_LOGV(tag,fmt,...) xjni_log_print(XJNI_LOG_VERBOSE,tag,fmt,##__VA_ARGS__)
#define XJNI_LOGD(tag,fmt,...) xjni_log_print(XJNI_LOG_DEBUG,tag,fmt,##__VA_ARGS__)
#define XJNI_LOGW(tag,fmt,...) xjni_log_print(XJNI_LOG_WARN, tag,fmt,##__VA_ARGS__)
#define XJNI_LOGF(tag,fmt,...) xjni_log_print(XJNI_LOG_FATAL,tag,fmt,##__VA_ARGS__)

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
// with colors
int xjni_log_vccprint(int prio,int line,const char* file,const char* tag,const char* fmt,va_list ap);
int xjni_log_ccprint(int prio,int line,const char* file,const char* tag,const char* fmt,...);
#endif

int xjni_log_vcprint(int prio,int line,const char* file,const char* tag,const char* fmt,va_list ap);
int xjni_log_cprint(int prio,int line,const char* file,const char* tag,const char* fmt,...);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_LOG_H__ */
