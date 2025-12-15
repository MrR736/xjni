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

#include <jni.h>

#include <xjni_arrayfield.h>
#include <xjni_stringarray.h>
#include <xjni2d.h>

#ifndef _XJNI_VERSION_MAJOR
#define _XJNI_VERSION_MAJOR	1
#endif
#ifndef _XJNI_VERSION_MINOR
#define _XJNI_VERSION_MINOR	0
#endif
#ifndef _XJNI_VERSION_PATCH
#define _XJNI_VERSION_PATCH	0
#endif

#define _XJNI_VERSION		((_XJNI_VERSION_MAJOR * 1000) + (_XJNI_VERSION_MINOR * 100) + _XJNI_VERSION_PATCH)

#ifdef __cplusplus
extern "C" {
#endif

extern const char* xjni_version(void);

extern void throwJava(JNIEnv *env, const char* tag, const char* msg, const char* cls);
extern void throwIOException(JNIEnv *env, const char* tag, const char* msg);
extern void throwCharConversionException(JNIEnv *env, const char* tag, const char* msg);
extern void throwEOFException(JNIEnv *env, const char* tag, const char* msg);
extern void throwFileNotFoundException(JNIEnv *env, const char* tag, const char* msg);
extern void throwInterruptedIOException(JNIEnv *env, const char* tag, const char* msg);
extern void throwInvalidClassException(JNIEnv *env, const char* tag, const char* msg);
extern void throwInvalidObjectException(JNIEnv *env, const char* tag, const char* msg);
extern void throwNotActiveException(JNIEnv *env, const char* tag, const char* msg);
extern void throwNotSerializableException(JNIEnv *env, const char* tag, const char* msg);
extern void throwOptionalDataException(JNIEnv *env, const char* tag, const char* msg);
extern void throwStreamCorruptedException(JNIEnv *env, const char* tag, const char* msg);
extern void throwSyncFailedException(JNIEnv *env, const char* tag, const char* msg);
extern void throwUTFDataFormatException(JNIEnv *env, const char* tag, const char* msg);
extern void throwUnsupportedEncodingException(JNIEnv *env, const char* tag, const char* msg);
extern void throwWriteAbortedException(JNIEnv *env, const char* tag, const char* msg);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_H__ */
