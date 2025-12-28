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

extern int vsjprintf(jchar * __s,const jchar * __format,va_list __arg);
extern int sjprintf(jchar * __s, const jchar * __format,...);

extern int vsnjprintf(jchar * __s,size_t __maxlen,const jchar * __format,va_list __arg);
extern int snjprintf(jchar * __s,size_t __maxlen,const jchar * __format, ...);

extern int vjfprintf(FILE* __stream, const jchar* __format, va_list __arg);
extern int jfprintf(FILE* __stream, const jchar* __format, ...);

extern int vjdprintf(int __fd, const jchar* __format, va_list __arg);
extern int jdprintf(int __fd, const jchar* __format, ...);

extern int vjprintf(const jchar* __format, va_list __arg);
extern int jprintf(const jchar* __format, ...);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_PRINTF_H__ */
