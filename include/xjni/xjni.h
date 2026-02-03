/**
 * xjni.h: Extern JNI Utility
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

#ifndef __XJNI_H__
#define __XJNI_H__

#include <stdio.h>
#include <stddef.h>
#include <pthread.h>
#include <jni.h>

#include <xjni_new.h>
#include <xjni_printf.h>
#include <xjni_arrayfield.h>
#include <xjni_stringarray.h>
#include <xjni_stringbuilder.h>
#include <xjni_stringbuffer.h>
#include <xjni_stringreader.h>
#include <xjni_stringwriter.h>
#include <xjni_va_list.h>
#include <xjni_log.h>
#include <xjni2d.h>

#ifndef _XJNI_VERSION_MAJOR
#define _XJNI_VERSION_MAJOR	1
#endif
#ifndef _XJNI_VERSION_MINOR
#define _XJNI_VERSION_MINOR	0
#endif
#ifndef _XJNI_VERSION_PATCH
#define _XJNI_VERSION_PATCH	6
#endif

#define _XJNI_VERSION		((_XJNI_VERSION_MAJOR * 1000) + (_XJNI_VERSION_MINOR * 100) + _XJNI_VERSION_PATCH)

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT const char* JNICALL xjni_version(void);

JNIEXPORT jint JNICALL xjni_tochar(const jchar c, char* out, size_t out_size);
JNIEXPORT jint JNICALL xjni_tojchar(const char* utf8, size_t utf8_len, jchar* out);

JNIEXPORT char* JNICALL xjni_tostring(const jchar* c);
JNIEXPORT jchar* JNICALL xjni_tojstring(const char* c);
JNIEXPORT jboolean JNICALL xjni_fromstring(const char *src,jchar *dst,size_t *dstlen);
JNIEXPORT jboolean JNICALL xjni_fromjstring(const jchar *src,char *dst,size_t *dstlen);

// JNI Utility
JNIEXPORT void JNICALL FatalErrorV(JNIEnv *env,const char *msg,va_list __arg);
JNIEXPORT void JNICALL FatalErrorF(JNIEnv *env,const char *msg,...);

JNIEXPORT jint JNICALL ThrowNewV(JNIEnv *env,jclass clazz,const char *msg,va_list __arg);
JNIEXPORT jint JNICALL ThrowNewF(JNIEnv *env,jclass clazz,const char *msg,...);

// Log Error Utility
JNIEXPORT jint JNICALL XJNI_OnLoad(JavaVM* vm,void* reserved,jint ver);
JNIEXPORT void JNICALL XJNI_OnUnload(JavaVM* vm,void* reserved,jint ver);

JNIEXPORT void JNICALL throwJava(JNIEnv *env,const char* tag,const char* msg,const char* cls_name,jclass* cache,pthread_mutex_t* mutex);
JNIEXPORT void JNICALL
throwJavaV(JNIEnv *env,const char* tag,const char* cls_name,jclass* cache,pthread_mutex_t* mutex,const char* msg,va_list ap);
JNIEXPORT void JNICALL
throwJavaF(JNIEnv *env,const char* tag,const char* cls_name,jclass* cache,pthread_mutex_t* mutex,const char *msg,...);

JNIEXPORT void JNICALL throwIOException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwCharConversionException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwEOFException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwFileNotFoundException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwInterruptedIOException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwInvalidClassException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwInvalidObjectException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwNotActiveException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwNotSerializableException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwOptionalDataException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwStreamCorruptedException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwSyncFailedException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwUTFDataFormatException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwUnsupportedEncodingException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwWriteAbortedException(JNIEnv *env,const char* tag,const char* msg);
JNIEXPORT void JNICALL throwOutOfMemoryError(JNIEnv *env,const char* tag,const char* msg);

JNIEXPORT void JNICALL throwIOExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwCharConversionExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwEOFExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwFileNotFoundExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwInterruptedIOExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwInvalidClassExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwInvalidObjectExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwNotActiveExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwNotSerializableExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwOptionalDataExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwStreamCorruptedExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwSyncFailedExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwUTFDataFormatExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwUnsupportedEncodingExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwWriteAbortedExceptionV(JNIEnv *env,const char* tag,const char* msg,va_list ap);
JNIEXPORT void JNICALL throwOutOfMemoryErrorV(JNIEnv *env,const char* tag,const char* msg,va_list ap);

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

// memory Utility
JNIEXPORT void* JNICALL jmemcpy(void *dest,const void *src,size_t n);
JNIEXPORT void* JNICALL jmemmove(void *dest,const void *src,size_t n);
JNIEXPORT void* JNICALL jmemchr(const void *s,jint c,size_t n);
JNIEXPORT void* JNICALL jmemset(void *s,jint c,size_t n);
JNIEXPORT jint JNICALL jmemcmp(const void *cs,const void *ct,size_t count);

// string Utility
JNIEXPORT size_t JNICALL jstrlen(const jchar* __s);
JNIEXPORT size_t JNICALL jstrnlen(const jchar * s,size_t count);
JNIEXPORT jchar* JNICALL jstrchr(const jchar *s,jint c);
JNIEXPORT jchar* JNICALL jstrchrnul(const jchar *__s,jint __c);
JNIEXPORT jchar* JNICALL jstrrchr(const jchar *s,jint c);
JNIEXPORT jint JNICALL jstrcmp(const jchar *cs,const jchar *ct);
JNIEXPORT jchar* JNICALL jstrcpy(jchar* __dest,const jchar* __src);
JNIEXPORT size_t JNICALL jstrlcpy(jchar *dest,const jchar *src,size_t size);
JNIEXPORT jchar* JNICALL jstrcat(jchar* __dest,const jchar* __src);
JNIEXPORT size_t JNICALL jstrlcat(jchar *dest,const jchar *src,size_t size);
JNIEXPORT jint JNICALL jstrncmp(const jchar *cs,const jchar *ct,size_t count);
JNIEXPORT jchar* JNICALL jstrpbrk(const jchar * cs,const jchar * ct);
JNIEXPORT jchar* JNICALL jstrstr(const jchar *s1,const jchar *s2);
JNIEXPORT size_t JNICALL jstrcspn(const jchar *__s,const jchar *__reject);
JNIEXPORT size_t JNICALL jstrspn(const jchar *s,const jchar *accept);
JNIEXPORT jchar* JNICALL jstrdup(const jchar *s);
JNIEXPORT jchar* JNICALL jstrndup(const jchar *__string,size_t __n);
JNIEXPORT jint JNICALL jstrcoll(const jchar *__s1,const jchar *__s2);
JNIEXPORT size_t JNICALL jstrxfrm(jchar* __dest,const jchar* __src,size_t __n);
JNIEXPORT jchar* JNICALL jstrtok(jchar* __s,const jchar* __delim);
JNIEXPORT void JNICALL jstrreverse(jchar* __str);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_H__ */
