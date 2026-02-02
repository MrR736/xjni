/**
 * xjni_printf.h: Extern JNI Printf Utility
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

#ifndef __XJNI_PRINTF_H__
#define __XJNI_PRINTF_H__

#include <stdio.h>
#include <stdarg.h>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT int JNICALL vsjprintf(jchar * __s,const jchar * __format,va_list __arg);
JNIEXPORT int JNICALL sjprintf(jchar * __s, const jchar * __format,...);

JNIEXPORT int JNICALL vsnjprintf(jchar * __s,size_t __maxlen,const jchar * __format,va_list __arg);
JNIEXPORT int JNICALL snjprintf(jchar * __s,size_t __maxlen,const jchar * __format, ...);

JNIEXPORT int JNICALL vjfprintf(FILE* __stream, const jchar* __format, va_list __arg);
JNIEXPORT int JNICALL jfprintf(FILE* __stream, const jchar* __format, ...);

JNIEXPORT int JNICALL vjdprintf(int __fd, const jchar* __format, va_list __arg);
JNIEXPORT int JNICALL jdprintf(int __fd, const jchar* __format, ...);

JNIEXPORT int JNICALL vjprintf(const jchar* __format, va_list __arg);
JNIEXPORT int JNICALL jprintf(const jchar* __format, ...);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_PRINTF_H__ */
