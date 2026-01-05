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

#ifdef __ANDROID__
#include <android/log.h>
#endif

#ifndef LOG_TAG
#define LOG_TAG "APP"
#endif

#define BASE_JNI_VERSION_MAJOR	1
#define BASE_JNI_VERSION_MINOR	0
#define BASE_JNI_VERSION_PATCH	1
#define BASE_JNI_VERSION	((BASE_JNI_VERSION_MAJOR * 1000) + (BASE_JNI_VERSION_MINOR * 100) + BASE_JNI_VERSION_PATCH)
#define BASE_JNI_STRVERSION	"1.0.1"

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
#define __BASE_GNUC_PREREQ(maj,min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
#define __BASE_GNUC_PREREQ(maj,min) 0
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
#define JNIEXPORTC extern "C" JNIEXPORT

#define BASEJNIO(__func,env)		(env->__func())
#define BASEJNIC(__func,env,...)	(env->__func(__VA_ARGS__))
#else
#define BASE_EXTERN_CXX
#define BASE_EXTERN_C
#define BASE_EXTERN_START
#define BASE_EXTERN_END
#define JNIEXPORTC JNIEXPORT

#define BASEJNIO(__func,env)		((*env)->__func(env))
#define BASEJNIC(__func,env,...)	((*env)->__func(env,__VA_ARGS__))
#endif

#define _GetMethodID(env,ex,name,sig) BASEJNIC(GetMethodID,env,ex,name,sig)
#define _GetStaticMethodID(env,ex,name,sig) BASEJNIC(GetStaticMethodID,env,ex,name,sig)
#define _GetFieldID(env,ex,name,sig) BASEJNIC(GetFieldID,env,ex,name,sig)
#define _GetStaticFieldID(env,ex,name,sig) BASEJNIC(GetStaticFieldID,env,ex,name,sig)
#define _FindClass(env,ex) BASEJNIC(FindClass,env,ex)
#define _GetArrayLength(env,ex) BASEJNIC(GetArrayLength,env,ex)
#define _ThrowNew(env,ex,msg) BASEJNIC(ThrowNew,env,ex,msg)

// Char
#define _NewCharArray(env,len) BASEJNIC(NewCharArray,env,len)
#define _CallCharMethod(env,ex,methodID) BASEJNIC(CallCharMethod,env,ex,methodID)
#define _CallCharMethod_l(env,ex,methodID,...) BASEJNIC(CallCharMethod,env,ex,methodID,__VA_ARGS__)
#define _CallNonvirtualCharMethod(env,ex,clazz,methodID) BASEJNIC(CallNonvirtualCharMethod,env,ex,clazz,methodID)
#define _CallNonvirtualCharMethod_l(env,ex,clazz,methodID,...) BASEJNIC(CallNonvirtualCharMethod,env,ex,clazz,methodID,__VA_ARGS__)
#define _CallStaticCharMethod(env,ex,methodID) BASEJNIC(CallStaticCharMethod,env,ex,methodID)
#define _CallStaticCharMethod_l(env,ex,methodID,...) BASEJNIC(CallStaticCharMethod,env,ex,methodID,__VA_ARGS__)

// Void
#define _CallVoidMethod(env,ex,methodID) BASEJNIC(CallVoidMethod,env,ex,methodID)
#define _CallVoidMethod_l(env,ex,methodID,...) BASEJNIC(CallVoidMethod,env,ex,methodID,__VA_ARGS__)
#define _CallStaticVoidMethod(env,ex,methodID) BASEJNIC(CallStaticVoidMethod,env,ex,methodID)
#define _CallStaticVoidMethod_l(env,ex,methodID,...) BASEJNIC(CallStaticVoidMethod,env,ex,methodID,__VA_ARGS__)
#define _CallNonvirtualVoidMethod(env,ex,clazz,methodID) BASEJNIC(CallNonvirtualVoidMethod,env,ex,clazz,methodID)
#define _CallNonvirtualVoidMethod_l(env,ex,clazz,methodID,...) BASEJNIC(CallNonvirtualVoidMethod,env,ex,clazz,methodID,__VA_ARGS__)

// Object
#define _GetObjectClass(env,ex) BASEJNIC(GetObjectClass,env,ex)
#define _NewObject(env,ex,methodID) BASEJNIC(NewObject,env,ex,methodID)
#define _NewObjectV(env,clazz,methodID,ap) BASEJNIC(NewObject,env,clazz,methodID,ap)
#define _NewObjectA(env,clazz,methodID,args) BASEJNIC(NewObjectA,env,clazz,methodID,args)
#define _NewObject_l(env,ex,methodID,...) BASEJNIC(NewObject,env,ex,methodID,__VA_ARGS__)
#define _CallStaticObjectMethod(env,ex,methodID) BASEJNIC(CallStaticObjectMethod,env,ex,methodID)
#define _CallStaticObjectMethod_l(env,ex,methodID,...) BASEJNIC(CallStaticObjectMethod,env,ex,methodID,__VA_ARGS__)
#define _GetObjectField(env,ex,methodID) BASEJNIC(GetObjectField,env,ex,methodID)
#define _GetStaticObjectField(env,ex,methodID) BASEJNIC(GetStaticObjectField,env,ex,methodID)
#define _CallObjectMethod(env,ex,methodID) BASEJNIC(CallObjectMethod,env,ex,methodID)
#define _CallObjectMethod_l(env,ex,methodID,...) BASEJNIC(CallObjectMethod,env,ex,methodID,__VA_ARGS__)
#define _SetObjectField(env,ex,methodID,val) BASEJNIC(SetObjectField,env,ex,methodID,val)
#define _SetStaticObjectField(env,ex,methodID,val) BASEJNIC(SetStaticObjectField,env,ex,methodID,val)
#define _GetObjectArrayElement(env,array,jindex) BASEJNIC(GetObjectArrayElement,env,array,jindex)
#define _SetObjectArrayElement(env,array,index,val) BASEJNIC(SetObjectArrayElement,env,array,index,val)
#define _NewObjectArray(env,len,clazz,init) BASEJNIC(NewObjectArray,env,len,clazz,init)

// Double
#define _NewDoubleArray(env,len) BASEJNIC(NewDoubleArray,env,len)
#define _GetDoubleField(env,clazz,fieldID) BASEJNIC(GetDoubleField,env,clazz,fieldID)
#define _GetStaticDoubleField(env,clazz,fieldID) BASEJNIC(GetStaticDoubleField,env,clazz,fieldID)
#define _GetDoubleArrayRegion(env,array,start,len,buf) BASEJNIC(GetDoubleArrayRegion,env,array,start,len,buf)
#define _SetDoubleArrayRegion(env,array,start,len,buf) BASEJNIC(SetDoubleArrayRegion,env,array,start,len,buf)

// Short
#define _NewShortArray(env,len) BASEJNIC(NewShortArray,env,len)
#define _GetShortField(env,clazz,fieldID) BASEJNIC(GetShortField,env,clazz,fieldID)
#define _SetShortField(env,clazz,fieldID,val) BASEJNIC(SetShortField,env,clazz,fieldID,val)
#define _GetStaticShortField(env,clazz,fieldID) BASEJNIC(GetStaticShortField,env,clazz,fieldID)
#define _GetShortArrayElements(env,src,iscopy) BASEJNIC(GetShortArrayElements,env,src,iscopy)
#define _ReleaseShortArrayElements(env,array,_s,mode) BASEJNIC(ReleaseShortArrayElements,env,array,_s,mode)
#define _GetShortArrayRegion(env,array,start,len,buf) BASEJNIC(GetShortArrayRegion,env,array,start,len,buf)
#define _SetShortArrayRegion(env,array,start,len,buf) BASEJNIC(SetShortArrayRegion,env,array,start,len,buf)

// Float
#define _NewFloatArray(env,len) BASEJNIC(NewFloatArray,env,len)
#define _SetFloatField(env,clazz,fieldID) BASEJNIC(SetFloatField,env,clazz,fieldID)
#define _CallFloatMethod(env,ex,methodID) BASEJNIC(CallFloatMethod,env,ex,methodID)
#define _CallFloatMethod_l(env,ex,methodID,...) BASEJNIC(CallFloatMethod,env,ex,methodID,__VA_ARGS__)
#define _ReleaseFloatArrayElements(env,array,jin,mode) BASEJNIC(ReleaseFloatArrayElements,env,array,jin,mode)
#define _GetFloatArrayRegion(env,array,start,len,buf) BASEJNIC(GetFloatArrayRegion,env,array,start,len,buf)
#define _SetFloatArrayRegion(env,array,start,len,buf) BASEJNIC(SetFloatArrayRegion,env,array,start,len,buf)

// Boolean
#define _NewBooleanArray(env,len) BASEJNIC(NewBooleanArray,env,len)
#define _CallBooleanMethod(env,ex,methodID) BASEJNIC(CallBooleanMethod,env,ex,methodID)
#define _CallBooleanMethod_l(env,ex,methodID,...) BASEJNIC(CallBooleanMethod,env,ex,methodID,__VA_ARGS__)
#define _CallStaticBooleanMethod(env,ex,methodID) BASEJNIC(CallStaticBooleanMethod,env,ex,methodID)
#define _CallStaticBooleanMethod_l(env,ex,methodID,...) BASEJNIC(CallStaticBooleanMethod,env,ex,methodID,__VA_ARGS__)
#define _CallNonvirtualBooleanMethod(env,ex,methodID) BASEJNIC(CallNonvirtualBooleanMethod,env,ex,methodID)
#define _CallNonvirtualBooleanMethod_l(env,ex,methodID,...) BASEJNIC(CallNonvirtualBooleanMethod,env,ex,methodID,__VA_ARGS__)
#define _GetBooleanArrayElements(env,src,iscopy) BASEJNIC(GetBooleanArrayElements,env,src,iscopy)
#define _GetBooleanArrayRegion(env,array,start,len,buf) BASEJNIC(GetBooleanArrayRegion,env,array,start,len,buf)
#define _SetBooleanField(env,obj,jid,val) BASEJNIC(SetBooleanField,env,obj,jid,val)
#define _SetStaticBooleanField(env,obj,jid,val) BASEJNIC(SetStaticBooleanField,env,obj,jid,val)
#define _ReleaseBooleanArrayElements(env,array,_bool,mode) BASEJNIC(ReleaseBooleanArrayElements,env,array,_bool,mode)

// Long
#define _NewLongArray(env,len) BASEJNIC(NewLongArray,env,len)
#define _GetStaticLongField(env,clazz,fieldID) BASEJNIC(GetStaticLongField,env,clazz,fieldID)
#define _CallLongMethod(env,ex,methodID) BASEJNIC(CallLongMethod,env,ex,methodID)
#define _CallLongMethod_l(env,ex,methodID,...) BASEJNIC(CallLongMethod,env,ex,methodID,__VA_ARGS__)
#define _CallNonvirtualLongMethod(env,ex,methodID) BASEJNIC(CallNonvirtualLongMethod,env,ex,methodID)
#define _CallNonvirtualLongMethod_l(env,ex,methodID,...) BASEJNIC(CallNonvirtualLongMethod,env,ex,methodID,__VA_ARGS__)
#define _GetLongField(env,clazz,fieldID) BASEJNIC(GetLongField,env,clazz,fieldID)
#define _SetLongField(env,obj,fieldID,val) BASEJNIC(SetLongField,env,obj,fieldID,val)
#define _SetStaticLongField(env,obj,fieldID,val) BASEJNIC(SetStaticLongField,env,obj,fieldID,val)
#define _GetLongArrayElements(env,src,iscopy) BASEJNIC(GetLongArrayElements,env,src,iscopy)
#define _SetLongArrayRegion(env,array,start,len,buf) BASEJNIC(SetLongArrayRegion,env,array,start,len,buf)
#define _GetLongArrayRegion(env,array,start,len,buf) BASEJNIC(GetLongArrayRegion,env,array,start,len,buf)
#define _ReleaseLongArrayElements(env,array,_long,mode) BASEJNIC(ReleaseLongArrayElements,env,array,_long,mode)

// String
#define _NewString(env,src,len) BASEJNIC(NewString,env,src,len)
#define _GetStringChars(env,src,iscopy) BASEJNIC(GetStringChars,env,src,iscopy)
#define _ReleaseStringChars(env,src,out) BASEJNIC(ReleaseStringChars,env,src,out)
#define _GetStringLength(env,src) BASEJNIC(GetStringLength,env,src)

// StringUTF
#define _NewStringUTF(env,src) BASEJNIC(NewStringUTF,env,src)
#define _GetStringUTFRegion(env,array,start,len,buf) BASEJNIC(GetStringUTFRegion,env,array,start,len,buf)
#define _GetStringUTFLength(env,src) BASEJNIC(GetStringUTFLength,env,src)
#define _GetStringUTFChars(env,src,iscopy) BASEJNIC(GetStringUTFChars,env,src,iscopy)
#define _ReleaseStringUTFChars(env,src,out) BASEJNIC(ReleaseStringUTFChars,env,src,out)

// Byte
#define _GetByteField(env,clazz,fieldID) BASEJNIC(GetByteField,env,clazz,fieldID)
#define _GetStaticByteField(env,clazz,fieldID) BASEJNIC(GetStaticByteField,env,clazz,fieldID)
#define _NewByteArray(env,len) BASEJNIC(NewByteArray,env,len)
#define _GetByteArrayRegion(env,array,start,len,buf) BASEJNIC(GetByteArrayRegion,env,array,start,len,buf)
#define _SetByteArrayRegion(env,array,start,len,buf) BASEJNIC(SetByteArrayRegion,env,array,start,len,buf)
#define _GetByteArrayElements(env,src,iscopy) BASEJNIC(GetByteArrayElements,env,src,iscopy)
#define _ReleaseByteArrayElements(env,array,byte,mode) BASEJNIC(ReleaseByteArrayElements,env,array,byte,mode)

// Int
#define _NewIntArray(env,len) BASEJNIC(NewIntArray,env,len)
#define _GetIntField(env,clazz,fieldID) BASEJNIC(GetIntField,env,clazz,fieldID)
#define _SetIntField(env,obj,fieldID,val) BASEJNIC(SetIntField,env,obj,fieldID,val)
#define _SetStaticIntField(env,obj,fieldID,val) BASEJNIC(SetStaticIntField,env,obj,fieldID,val)
#define _CallIntMethod(env,ex,methodID) BASEJNIC(CallIntMethod,env,ex,methodID)
#define _CallIntMethod_l(env,ex,methodID,...) BASEJNIC(CallIntMethod,env,ex,methodID,__VA_ARGS__)
#define _ReleaseIntArrayElements(env,array,byte,mode) BASEJNIC(ReleaseIntArrayElements,env,array,byte,mode)
#define _SetIntArrayRegion(env,array,start,len,buf) BASEJNIC(SetIntArrayRegion,env,array,start,len,buf)
#define _GetIntArrayElements(env,src,iscopy) BASEJNIC(GetIntArrayElements,env,src,iscopy)

#define _NewGlobalRef(env,f) BASEJNIC(NewGlobalRef,env,f)
#define _DeleteGlobalRef(env,f) BASEJNIC(DeleteGlobalRef,env,f)
#define _DeleteLocalRef(env,f) BASEJNIC(DeleteLocalRef,env,f)
#define _FatalError(env,msg) BASEJNIC(FatalError,env,msg)
#define _NewLocalRef(env,ref) BASEJNIC(NewLocalRef,env,ref)
#define _GetSuperclass(env,sub) BASEJNIC(GetSuperclass,env,sub)
#define _IsAssignableFrom(env,sub,sup) BASEJNIC(IsAssignableFrom,env,sub,sup)
#define _GetEnv(vm,env,ver) BASEJNIC(GetEnv,vm,env,ver)

#define _ExceptionClear(env) BASEJNIO(ExceptionClear,env)
#define _ExceptionOccurred(env) BASEJNIO(ExceptionOccurred,env)
#define _ExceptionCheck(env) BASEJNIO(ExceptionCheck,env)
#define _ExceptionDescribe(env) BASEJNIO(ExceptionDescribe,env)


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
#define BASE_TAG_LOGE(tag,fmt,...) __android_log_print(ANDROID_LOG_ERROR,tag,fmt,##__VA_ARGS__)
#else
#define BASE_TAG_LOGE(tag,fmt,...) fprintf(stderr,"[%s] ERROR: " fmt,tag, ##__VA_ARGS__), fprintf(stderr,"\n")
#endif

#define BASE_LOGE(fmt,...) BASE_TAG_LOGE(LOG_TAG,fmt,##__VA_ARGS__)

#endif // BASE_JNI_H
