/**
 * xjni_va_list.h: Extern JNI VA LIST Utility
 *
 * Copyright (C) 2026 MrR736 <MrR736@users.github.com>
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

#ifndef __XJNI_VA_LIST_H__
#define __XJNI_VA_LIST_H__

#include <stdio.h>
#include <jni.h>
#include <xjni_args.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL JSnPrintf(JNIEnv *env,char* s,size_t maxlen,jstring format,jargs_t args);
JNIEXPORT void JNICALL JSnPrintfUTF(JNIEnv *env,char* s,size_t maxlen,const char* format,jargs_t args);

JNIEXPORT void JNICALL JSPrintf(JNIEnv *env,char* s,jstring format,jargs_t args);
JNIEXPORT void JNICALL JSPrintfUTF(JNIEnv *env,char* s,const char* format,jargs_t args);

JNIEXPORT void JNICALL JFPrintf(JNIEnv *env,FILE* fp,jstring format, jargs_t args);
JNIEXPORT void JNICALL JFPrintfUTF(JNIEnv *env,FILE* fp,const char* format,jargs_t args);

JNIEXPORT void JNICALL JDPrintf(JNIEnv *env,int fd,jstring format, jargs_t args);
JNIEXPORT void JNICALL JDPrintfUTF(JNIEnv *env,int fd,const char* format,jargs_t args);

JNIEXPORT void JNICALL JPrintf(JNIEnv *env,jstring format, jargs_t args);
JNIEXPORT void JNICALL JPrintfUTF(JNIEnv *env,const char* format, jargs_t args);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_VA_LIST_H__ */
