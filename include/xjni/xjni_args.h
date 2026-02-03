/**
 * xjni_args.h: Extern JNI Args Utility
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

#ifndef __XJNI_ARGS_H__
#define __XJNI_ARGS_H__

#include <jni.h>

typedef jobjectArray jargs_t;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jargs_t JNICALL NewJArgs(JNIEnv *env, jsize index,jclass cls,jobject init);

JNIEXPORT void JNICALL JArgsAppendObject(JNIEnv *env, jargs_t args, jobject obj);
JNIEXPORT void JNICALL JArgsAppendString(JNIEnv *env, jargs_t args, jstring obj);
JNIEXPORT void JNICALL JArgsAppendStringUTF(JNIEnv *env, jargs_t args,const char* fmt);
JNIEXPORT void JNICALL JArgsAppendChar(JNIEnv *env, jargs_t args, jchar obj);
JNIEXPORT void JNICALL JArgsAppendBoolean(JNIEnv *env, jargs_t args, jboolean obj);
JNIEXPORT void JNICALL JArgsAppendInt(JNIEnv *env, jargs_t args, jint obj);
JNIEXPORT void JNICALL JArgsAppendLong(JNIEnv *env, jargs_t args, jlong obj);
JNIEXPORT void JNICALL JArgsAppendFloat(JNIEnv *env, jargs_t args, jfloat obj);
JNIEXPORT void JNICALL JArgsAppendDouble(JNIEnv *env, jargs_t args, jdouble obj);

JNIEXPORT void JNICALL JArgsInsertObject(JNIEnv *env, jargs_t args, jobject obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertString(JNIEnv *env, jargs_t args, jstring obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertStringUTF(JNIEnv *env, jargs_t args,const char* fmt, jsize index);
JNIEXPORT void JNICALL JArgsInsertChar(JNIEnv *env, jargs_t args, jchar obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertBoolean(JNIEnv *env, jargs_t args, jboolean obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertInt(JNIEnv *env, jargs_t args, jint obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertLong(JNIEnv *env, jargs_t args, jlong obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertFloat(JNIEnv *env, jargs_t args, jfloat obj, jsize index);
JNIEXPORT void JNICALL JArgsInsertDouble(JNIEnv *env, jargs_t args, jdouble obj, jsize index);

JNIEXPORT void JNICALL JArgsReplaceObject(JNIEnv *env, jargs_t args, jobject obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceString(JNIEnv *env, jargs_t args, jstring obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceStringUTF(JNIEnv *env, jargs_t args,const char* fmt, jsize index);
JNIEXPORT void JNICALL JArgsReplaceChar(JNIEnv *env, jargs_t args, jchar obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceBoolean(JNIEnv *env, jargs_t args, jboolean obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceInt(JNIEnv *env, jargs_t args, jint obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceLong(JNIEnv *env, jargs_t args, jlong obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceFloat(JNIEnv *env, jargs_t args, jfloat obj, jsize index);
JNIEXPORT void JNICALL JArgsReplaceDouble(JNIEnv *env, jargs_t args, jdouble obj, jsize index);

JNIEXPORT void JNICALL JArgsDelete(JNIEnv *env, jargs_t args, jsize index);

JNIEXPORT jobject JNICALL GetJArgs(JNIEnv *env, jargs_t args, jsize index);
JNIEXPORT jstring JNICALL GetJArgsString(JNIEnv *env, jargs_t args, jsize index);
/* Returns heap-allocated modified UTF-8. Caller MUST free(). */
JNIEXPORT char* JNICALL GetJArgsStringUTF(JNIEnv *env, jargs_t args, jsize index);
JNIEXPORT jchar JNICALL GetJArgsChar(JNIEnv *env, jargs_t args, jsize index);
JNIEXPORT jboolean JNICALL GetJArgsBoolean(JNIEnv *env, jargs_t args, jsize index);
JNIEXPORT jlong JNICALL GetJArgsLong(JNIEnv *env, jargs_t args, jsize index);
JNIEXPORT jfloat JNICALL GetJArgsFloat(JNIEnv *env, jargs_t args, jsize index);
JNIEXPORT jdouble JNICALL GetJArgsDouble(JNIEnv *env, jargs_t args, jsize index);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_ARGS_H__ */
