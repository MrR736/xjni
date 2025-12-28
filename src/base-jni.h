/**
 * Base JNI Header
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

#ifndef BASE_JNI_H
#define BASE_JNI_H

#include <stdio.h>
#include <stdint.h>
#include <jni.h>

#ifndef LOG_TAG
#define LOG_TAG "APP"
#endif

#ifdef __ANDROID__
#include <android/log.h>
#endif

#if defined(_MSC_VER) && _MSC_VER < 1900 && !defined(inline)
#define inline __inline
#endif

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif
#ifndef __declspec
#define __declspec(x)
#endif

#ifndef __BASE_GNUC_PREREQ
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __BASE_GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
#define __BASE_GNUC_PREREQ(maj, min) 0
#endif
#endif

/* Types for `void *' pointers.  */
#define base_c_cast(t,p)	((t)(intptr_t)(p))
#define ubase_c_cast(t,p)	((t)(uintptr_t)(p))

#ifdef __cplusplus
#define ibase_cast(t,p)	(*reinterpret_cast<t *>((p)))
#define cbase_cast(t,p)	(const_cast<t>((p)))
#define scbase_cast(t)	(const_cast<t>)
#define base_cast(t,p)	(static_cast<t>((p)))
#define ubase_cast(t,p)	(reinterpret_cast<t>((p)))
#if __BASE_GNUC_PREREQ(2,95)
#define sbase_cast(t)	(static_cast<t>)
#define rbase_cast(t)	(reinterpret_cast<t>)
#else
#define sbase_cast(t)	((t))
#define rbase_cast(t)	((t))
#endif
#else
#define ibase_cast(t,p)	(*(t *)(p))
#define base_cast	base_c_cast
#define ubase_cast	ubase_c_cast
#define cbase_cast(t,p)	((t)(p))
#define scbase_cast(t)	((t))
#define sbase_cast(t)	((t))
#define rbase_cast(t)	((t))
#endif

#if defined(_MSC_VER) && _MSC_VER < 1900 && !defined(inline)
#define inline __inline
#endif

#ifdef __cplusplus
#define BASE_EXTERN_CXX extern "C++"
#define BASE_EXTERN_C extern "C"
#define BASE_EXTERN_START extern "C" {
#define BASE_EXTERN_END   }

#define _GetMethodID(env, ex, name, sig) env->GetMethodID(ex, name, sig)
#define _GetStaticMethodID(env, ex, name, sig) env->GetStaticMethodID(ex, name, sig)
#define _GetFieldID(env, ex, name, sig) env->GetFieldID(ex, name, sig)
#define _ThrowNew(env, ex, msg) env->ThrowNew(ex, msg)
#define _ExceptionClear(env) env->ExceptionClear()
#define _ExceptionOccurred(env) env->ExceptionOccurred()
#define _FindClass(env, ex) env->FindClass(ex)
#define _GetArrayLength(env, ex) env->GetArrayLength(ex)

// Char
#define _NewCharArray(env, len) env->NewCharArray(len)

// Void
#define _CallVoidMethod(env, ex, methodID) env->CallVoidMethod(ex, methodID)
#define _CallVoidMethod_l(env, ex, methodID, ...) env->CallVoidMethod(ex, methodID, __VA_ARGS__)
#define _CallStaticVoidMethod(env, ex, methodID) env->CallStaticVoidMethod(ex, methodID)
#define _CallStaticVoidMethod_l(env, ex, methodID, ...) env->CallStaticVoidMethod(ex, methodID, __VA_ARGS__)

// Object
#define _GetObjectClass(env, ex) env->GetObjectClass(ex)
#define _CallObjectMethod(env, ex, methodID) env->CallObjectMethod(ex, methodID)
#define _CallObjectMethod_l(env, ex, methodID, ...) env->CallObjectMethod(ex, methodID, __VA_ARGS__)
#define _CallStaticObjectMethod(env, ex, methodID) env->CallStaticObjectMethod(ex, methodID)
#define _CallStaticObjectMethod_l(env, ex, methodID, ...) env->CallStaticObjectMethod(ex, methodID, __VA_ARGS__)
#define _NewObject(env, ex, methodID) env->NewObject(ex, methodID)
#define _NewObject_l(env, ex, methodID, ...) env->NewObject(ex, methodID, __VA_ARGS__)
#define _GetObjectField(env, ex, methodID) env->GetObjectField(ex, methodID)
#define _GetStaticObjectField(env, ex, methodID) env->GetStaticObjectField(ex, methodID)
#define _SetObjectField(env, ex, methodID, val) env->SetObjectField(ex, methodID, val)
#define _SetStaticObjectField(env, ex, methodID, val) env->SetStaticObjectField(ex, methodID, val)
#define _GetObjectArrayElement(env,array,jindex) env->GetObjectArrayElement(array,jindex)
#define _SetObjectArrayElement(env,array,index,val) env->SetObjectArrayElement(array,index,val)
#define _NewObjectArray(env,len,clazz,init) env->NewObjectArray(len,clazz,init)

// Double
#define _NewDoubleArray(env, len) env->NewDoubleArray(len)
#define _GetDoubleField(env, clazz, fieldID) env->GetDoubleField(clazz, fieldID)
#define _GetStaticDoubleField(env, clazz, fieldID) env->GetStaticDoubleField(clazz, fieldID)

// Short
#define _GetShortField(env, clazz, fieldID) env->GetShortField(clazz, fieldID)
#define _SetShortField(env, clazz, fieldID, val) env->SetShortField(clazz, fieldID, val)
#define _GetStaticShortField(env, clazz, fieldID) env->GetStaticShortField(clazz, fieldID)
#define _GetShortArrayElements(env, src, iscopy) env->GetShortArrayElements(src, iscopy)
#define _ReleaseShortArrayElements(env, array, _long, mode) env->ReleaseShortArrayElements(array, _long, mode)
#define _GetShortArrayRegion(env, array, start, len, buf) env->GetShortArrayRegion(array, start, len, buf)
#define _SetShortArrayRegion(env, array, start, len, buf) env->SetShortArrayRegion(array, start, len, buf)

// Float
#define _NewFloatArray(env, len) env->NewFloatArray(len)
#define _SetFloatField(env, clazz, fieldID) env->SetFloatField(clazz, fieldID)
#define _CallFloatMethod(env, ex, methodID) env->CallFloatMethod(ex, methodID)
#define _CallFloatMethod_l(env, ex, methodID, ...) env->CallFloatMethod(ex, methodID, __VA_ARGS__)
#define _ReleaseFloatArrayElements(env, array, jin, mode) env->ReleaseFloatArrayElements(array, jin, mode)

// Boolean
#define _NewBooleanArray(env, len) env->NewBooleanArray(len)
#define _CallBooleanMethod(env, ex, methodID) env->CallBooleanMethod(ex, methodID)
#define _CallBooleanMethod_l(env, ex, methodID, ...) env->CallBooleanMethod(ex, methodID, __VA_ARGS__)
#define _CallStaticBooleanMethod(env, ex, methodID) env->CallStaticBooleanMethod(ex, methodID)
#define _CallStaticBooleanMethod_l(env, ex, methodID, ...) env->CallStaticBooleanMethod(ex, methodID, __VA_ARGS__)
#define _CallNonvirtualBooleanMethod(env, ex, methodID) env->CallNonvirtualBooleanMethod(ex, methodID)
#define _CallNonvirtualBooleanMethod_l(env, ex, methodID, ...) env->CallNonvirtualBooleanMethod(ex, methodID, __VA_ARGS__)
#define _GetBooleanArrayElements(env, src, iscopy) env->GetBooleanArrayElements(src, iscopy)
#define _GetBooleanArrayRegion(env, array, start, len, buf) env->GetBooleanArrayRegion(array, start, len, buf)
#define _ReleaseBooleanArrayElements(env, array, _bool, mode) env->ReleaseBooleanArrayElements(array, _bool, mode)
#define _SetBooleanField(env, obj, jid, val) env->SetBooleanField(obj, jid, val)
#define _SetStaticBooleanField(env, obj, jid, val) env->SetStaticBooleanField(obj, jid, val)

// Long
#define _NewLongArray(env, len) env->NewLongArray(len)
#define _GetLongField(env, clazz, fieldID) env->GetLongField(clazz, fieldID)
#define _GetStaticByteField(env, clazz, fieldID) env->GetStaticByteField(clazz, fieldID)
#define _CallLongMethod(env, ex, methodID) env->CallLongMethod(ex, methodID)
#define _CallLongMethod_l(env, ex, methodID, ...) env->CallLongMethod(ex, methodID, __VA_ARGS__)
#define _CallNonvirtualLongMethod(env, ex, methodID) env->CallNonvirtualLongMethod(ex, methodID)
#define _CallNonvirtualLongMethod_l(env, ex, methodID, ...) env->CallNonvirtualLongMethod(ex, methodID, __VA_ARGS__)
#define _GetStaticLongField(env, clazz, fieldID) env->GetStaticLongField(clazz, fieldID)
#define _SetLongField(env, obj, fieldID, val) env->SetLongField(obj, fieldID, val)
#define _SetStaticLongField(env, obj, fieldID, val) env->SetStaticLongField(obj, fieldID, val)
#define _GetLongArrayElements(env, src, iscopy) env->GetLongArrayElements(src, iscopy)
#define _ReleaseLongArrayElements(env, array, _long, mode) env->ReleaseLongArrayElements(array, _long, mode)
#define _SetLongArrayRegion(env, array, start, len, buf) env->SetLongArrayRegion(array, start, len, buf)
#define _GetLongArrayRegion(env, array, start, len, buf) env->GetLongArrayRegion(array, start, len, buf)

// String
#define _NewString(env, src, len) env->NewString(src, len)
#define _GetStringChars(env, src, iscopy) env->GetStringChars(src, iscopy)
#define _ReleaseStringChars(env, src, out) env->ReleaseStringChars(src, out)
#define _GetStringLength(env, src) env->GetStringLength(src)

// StringUTF
#define _GetStringUTFRegion(env, array, start, len, buf) env->GetStringUTFRegion(array, start, len, buf)
#define _NewStringUTF(env, src) env->NewStringUTF(src)
#define _GetStringUTFLength(env, src) env->GetStringUTFLength(src)
#define _GetStringUTFChars(env, src, iscopy) env->GetStringUTFChars(src, iscopy)
#define _ReleaseStringUTFChars(env, src, out) env->ReleaseStringUTFChars(src, out)

// Byte
#define _NewByteArray(env, len) env->NewByteArray(len)
#define _GetByteField(env, clazz, fieldID) env->GetByteField(clazz, fieldID)
#define _GetByteArrayRegion(env, array, start, len, buf) env->GetByteArrayRegion(array, start, len, buf)
#define _SetByteArrayRegion(env, array, start, len, buf) env->SetByteArrayRegion(array, start, len, buf)
#define _GetByteArrayElements(env, src, iscopy) env->GetByteArrayElements(src, iscopy)
#define _ReleaseByteArrayElements(env, array, byte, mode) env->ReleaseByteArrayElements(array, byte, mode)

// Int
#define _NewIntArray(env, len) env->NewIntArray(len)
#define _GetIntField(env, clazz, fieldID) env->GetIntField(clazz, fieldID)
#define _SetIntField(env, obj, fieldID, val) env->SetIntField(obj, fieldID, val)
#define _SetStaticIntField(env, obj, fieldID, val) env->SetStaticIntField(obj, fieldID, val)
#define _CallIntMethod(env, ex, methodID) env->CallIntMethod(ex, methodID)
#define _CallIntMethod_l(env, ex, methodID, ...) env->CallIntMethod(ex, methodID, __VA_ARGS__)
#define _GetIntArrayElements(env, src, iscopy) env->GetIntArrayElements(src, iscopy)
#define _SetIntArrayRegion(env, array, start, len, buf) env->SetIntArrayRegion(array, start, len, buf)
#define _ReleaseIntArrayElements(env, array, byte, mode) env->ReleaseIntArrayElements(array, byte, mode)

#define _NewGlobalRef(env, f) env->NewGlobalRef(f)
#define _DeleteGlobalRef(env, f) env->DeleteGlobalRef(f)
#define _DeleteLocalRef(env, f) env->DeleteLocalRef(f)
#define _ExceptionCheck(env) env->ExceptionCheck()
#define _ExceptionDescribe(env) env->ExceptionDescribe()

#define _GetEnv(vm, env, ver) vm->GetEnv((void**)&env, ver)
#else
#define BASE_EXTERN_CXX
#define BASE_EXTERN_C
#define BASE_EXTERN_START
#define BASE_EXTERN_END
#define _GetMethodID(env, ex, name, sig) (*env)->GetMethodID(env, ex, name, sig)
#define _GetStaticMethodID(env, ex, name, sig) (*env)->GetStaticMethodID(env, ex, name, sig)
#define _GetFieldID(env, ex, name, sig) (*env)->GetFieldID(env, ex, name, sig)
#define _FindClass(env, ex) (*env)->FindClass(env, ex)
#define _GetArrayLength(env, ex) (*env)->GetArrayLength(env, ex)
#define _ThrowNew(env, ex, msg) (*env)->ThrowNew(env, ex, msg)
#define _ExceptionClear(env) (*env)->ExceptionClear(env)
#define _ExceptionOccurred(env) (*env)->ExceptionOccurred(env)
#define _NewCharArray(env, len) (*env)->NewCharArray(env, len)

// Void
#define _CallVoidMethod(env, ex, methodID) (*env)->CallVoidMethod(env, ex, methodID)
#define _CallVoidMethod_l(env, ex, methodID, ...) (*env)->CallVoidMethod(env, ex, methodID, __VA_ARGS__)
#define _CallStaticVoidMethod(env, ex, methodID) (*env)->CallStaticVoidMethod(env, ex, methodID)
#define _CallStaticVoidMethod_l(env, ex, methodID, ...) (*env)->CallStaticVoidMethod(env, ex, methodID, __VA_ARGS__)

// Object
#define _GetObjectClass(env, ex) (*env)->GetObjectClass(env, ex)
#define _NewObject(env, ex, methodID) (*env)->NewObject(env, ex, methodID)
#define _NewObject_l(env, ex, methodID, ...) (*env)->NewObject(env, ex, methodID, __VA_ARGS__)
#define _CallStaticObjectMethod(env, ex, methodID) (*env)->CallStaticObjectMethod(env, ex, methodID)
#define _CallStaticObjectMethod_l(env, ex, methodID, ...) (*env)->CallStaticObjectMethod(env, ex, methodID, __VA_ARGS__)
#define _GetObjectField(env, ex, methodID) (*env)->GetObjectField(env, ex, methodID)
#define _GetStaticObjectField(env, ex, methodID) (*env)->GetStaticObjectField(env, ex, methodID)
#define _CallObjectMethod(env, ex, methodID) (*env)->CallObjectMethod(env, ex, methodID)
#define _CallObjectMethod_l(env, ex, methodID, ...) (*env)->CallObjectMethod(env, ex, methodID, __VA_ARGS__)
#define _SetObjectField(env, ex, methodID, val) (*env)->SetObjectField(env,ex, methodID, val)
#define _SetStaticObjectField(env, ex, methodID, val) (*env)->SetStaticObjectField(env,ex, methodID, val)
#define _GetObjectArrayElement(env,array,jindex) (*env)->GetObjectArrayElement(env,array,jindex)
#define _SetObjectArrayElement(env,array,index,val) (*env)->SetObjectArrayElement(env,array,index,val)
#define _NewObjectArray(env,len,clazz,init) (*env)->NewObjectArray(env,len,clazz,init)

// Double
#define _NewDoubleArray(env, len) (*env)->NewDoubleArray(env, len)
#define _GetDoubleField(env, clazz, fieldID) (*env)->GetDoubleField(env, clazz, fieldID)
#define _GetStaticDoubleField(env, clazz, fieldID) (*env)->GetStaticDoubleField(env, clazz, fieldID)

// Short
#define _GetShortField(env, clazz, fieldID) (*env)->GetShortField(env, clazz, fieldID)
#define _SetShortField(env, clazz, fieldID, val) (*env)->SetShortField(env, clazz, fieldID, val)
#define _GetStaticShortField(env, clazz, fieldID) (*env)->GetStaticShortField(env, clazz, fieldID)
#define _GetShortArrayElements(env, src, iscopy) (*env)->GetShortArrayElements(env,src,iscopy)
#define _ReleaseShortArrayElements(env, array, _long, mode) (*env)->ReleaseShortArrayElements(env,array,_long,mode)
#define _GetShortArrayRegion(env, array, start, len, buf) (*env)->GetShortArrayRegion(env, array, start, len, buf)
#define _SetShortArrayRegion(env, array, start, len, buf) (*env)->SetShortArrayRegion(env, array, start, len, buf)

// Float
#define _NewFloatArray(env, len) (*env)->NewFloatArray(env, len)
#define _SetFloatField(env, clazz, fieldID) (*env)->SetFloatField(env, clazz, fieldID)
#define _CallFloatMethod(env, ex, methodID) (*env)->CallFloatMethod(env, ex, methodID)
#define _CallFloatMethod_l(env, ex, methodID, ...) (*env)->CallFloatMethod(env, ex, methodID, __VA_ARGS__)
#define _ReleaseFloatArrayElements(env, array, jin, mode) (*env)->ReleaseFloatArrayElements(env, array, jin, mode)

// Boolean
#define _NewBooleanArray(env, len) (*env)->NewBooleanArray(env, len)
#define _CallBooleanMethod(env, ex, methodID) (*env)->CallBooleanMethod(env, ex, methodID)
#define _CallBooleanMethod_l(env, ex, methodID, ...) (*env)->CallBooleanMethod(env, ex, methodID, __VA_ARGS__)
#define _CallStaticBooleanMethod(env, ex, methodID) (*env)->CallStaticBooleanMethod(env, ex, methodID)
#define _CallStaticBooleanMethod_l(env, ex, methodID, ...) (*env)->CallStaticBooleanMethod(env, ex, methodID, __VA_ARGS__)
#define _CallNonvirtualBooleanMethod(env, ex, methodID) (*env)->CallNonvirtualBooleanMethod(env, ex, methodID)
#define _CallNonvirtualBooleanMethod_l(env, ex, methodID, ...) (*env)->CallNonvirtualBooleanMethod(env, ex, methodID, __VA_ARGS__)
#define _GetBooleanArrayElements(env, src, iscopy) (*env)->GetBooleanArrayElements(env, src, iscopy)
#define _GetBooleanArrayRegion(env, array, start, len, buf) (*env)->GetBooleanArrayRegion(env, array, start, len, buf)
#define _SetBooleanField(env, obj, jid, val) (*env)->SetBooleanField(env, obj, jid, val)
#define _SetStaticBooleanField(env, obj, jid, val) (*env)->SetStaticBooleanField(env, obj, jid, val)
#define _ReleaseBooleanArrayElements(env, array, _bool, mode) (*env)->ReleaseBooleanArrayElements(env, array, _bool, mode)

// Long
#define _NewLongArray(env, len) (*env)->NewLongArray(env, len)
#define _GetStaticLongField(env, clazz, fieldID) (*env)->GetStaticLongField(env, clazz, fieldID)
#define _CallLongMethod(env, ex, methodID) (*env)->CallLongMethod(env, ex, methodID)
#define _CallLongMethod_l(env, ex, methodID, ...) (*env)->CallLongMethod(env, ex, methodID, __VA_ARGS__)
#define _CallNonvirtualLongMethod(env, ex, methodID) (*env)->CallNonvirtualLongMethod(env, ex, methodID)
#define _CallNonvirtualLongMethod_l(env, ex, methodID, ...) (*env)->CallNonvirtualLongMethod(env, ex, methodID, __VA_ARGS__)
#define _GetLongField(env, clazz, fieldID) (*env)->GetLongField(env, clazz, fieldID)
#define _SetLongField(env, obj, fieldID, val) (*env)->SetLongField(env, obj, fieldID, val)
#define _SetStaticLongField(env, obj, fieldID, val) (*env)->SetStaticLongField(env, obj, fieldID, val)
#define _GetLongArrayElements(env, src, iscopy) (*env)->GetLongArrayElements(env, src, iscopy)
#define _SetLongArrayRegion(env, array, start, len, buf) (*env)->SetLongArrayRegion(env, array, start, len, buf)
#define _GetLongArrayRegion(env, array, start, len, buf) (*env)->GetLongArrayRegion(env, array, start, len, buf)
#define _ReleaseLongArrayElements(env, array, _long, mode) (*env)->ReleaseLongArrayElements(env, array, _long, mode)

// String
#define _NewString(env, src, len) (*env)->NewString(env, src, len)
#define _GetStringChars(env, src, iscopy) (*env)->GetStringChars(env, src, iscopy)
#define _ReleaseStringChars(env, src, out) (*env)->ReleaseStringChars(env, src, out)
#define _GetStringLength(env, src) (*env)->GetStringLength(env, src)

// StringUTF
#define _NewStringUTF(env, src) (*env)->NewStringUTF(env, src)
#define _GetStringUTFRegion(env, array, start, len, buf) (*env)->GetStringUTFRegion(env, array, start, len, buf)
#define _GetStringUTFLength(env, src) (*env)->GetStringUTFLength(env, src)
#define _GetStringUTFChars(env, src, iscopy) (*env)->GetStringUTFChars(env, src, iscopy)
#define _ReleaseStringUTFChars(env, src, out) (*env)->ReleaseStringUTFChars(env, src, out)

// Byte
#define _GetByteField(env, clazz, fieldID) (*env)->GetByteField(env, clazz, fieldID)
#define _GetStaticByteField(env, clazz, fieldID) (*env)->GetStaticByteField(env, clazz, fieldID)
#define _NewByteArray(env, len) (*env)->NewByteArray(env, len)
#define _GetByteArrayRegion(env, array, start, len, buf) (*env)->GetByteArrayRegion(env, array, start, len, buf)
#define _SetByteArrayRegion(env, array, start, len, buf) (*env)->SetByteArrayRegion(env, array, start, len, buf)
#define _GetByteArrayElements(env, src, iscopy) (*env)->GetByteArrayElements(env, src, iscopy)
#define _ReleaseByteArrayElements(env, array, byte, mode) (*env)->ReleaseByteArrayElements(env, array, byte, mode)

// Int
#define _NewIntArray(env, len) (*env)->NewIntArray(env, len)
#define _GetIntField(env, clazz, fieldID) (*env)->GetIntField(env, clazz, fieldID)
#define _SetIntField(env, obj, fieldID, val) (*env)->SetIntField(env, obj, fieldID, val)
#define _SetStaticIntField(env, obj, fieldID, val) (*env)->SetStaticIntField(env, obj, fieldID, val)
#define _CallIntMethod(env, ex, methodID) (*env)->CallIntMethod(env, ex, methodID)
#define _CallIntMethod_l(env, ex, methodID, ...) (*env)->CallIntMethod(env, ex, methodID, __VA_ARGS__)
#define _ReleaseIntArrayElements(env, array, byte, mode) (*env)->ReleaseIntArrayElements(env, array, byte, mode)
#define _SetIntArrayRegion(env, array, start, len, buf) (*env)->SetIntArrayRegion(env, array, start, len, buf)
#define _GetIntArrayElements(env, src, iscopy) (*env)->GetIntArrayElements(env, src, iscopy)

#define _NewGlobalRef(env,f) (*env)->NewGlobalRef(env,f)
#define _DeleteGlobalRef(env,f) (*env)->DeleteGlobalRef(env,f)
#define _DeleteLocalRef(env,f) (*env)->DeleteLocalRef(env,f)
#define _ExceptionCheck(env) (*env)->ExceptionCheck(env)
#define _ExceptionDescribe(env) (*env)->ExceptionDescribe(env)

#define _GetEnv(vm, env, ver) (*vm)->GetEnv(vm, (void**)&env, ver)
#endif

#ifdef __cplusplus
#define JNIEXPORTC extern "C" JNIEXPORT
#else
#define JNIEXPORTC JNIEXPORT
#endif

#if (defined(__GNUC__) && (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 2)) || __has_attribute(visibility)
#ifdef ARM
#define BASE_VISIBILITY(V) __attribute__((externally_visible,visibility(#V)))
#else
#define BASE_VISIBILITY(V) __attribute__((visibility(#V)))
#endif
#else
#define BASE_VISIBILITY(V)
#endif

# ifdef __ANDROID__
#define BASE_LOGE(fmt, ...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,fmt, ##__VA_ARGS__)
#else
#define BASE_LOGE(fmt, ...) fprintf(stderr, "[" LOG_TAG "] ERROR: " fmt, ##__VA_ARGS__)
#endif

#endif // BASE_JNI_H
