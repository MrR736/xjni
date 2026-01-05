#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <jni.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

#define DEFINE_ARRAY_FIELD_GETTERS_SETTERS(type,jtype) \
JNIEXPORTC j##jtype##Array JNICALL Get##type##ArrayField(JNIEnv *env,jobject obj,jfieldID fid) { \
	return ubase_cast(j##jtype##Array,GetObjectArrayField(env,obj,fid)); \
} \
JNIEXPORTC j##jtype##Array JNICALL GetStatic##type##ArrayField(JNIEnv *env,jclass cls,jfieldID fid) { \
	return ubase_cast(j##jtype##Array,GetStaticObjectArrayField(env,cls,fid)); \
}\
JNIEXPORTC void JNICALL Set##type##ArrayField(JNIEnv *env,jobject obj,jfieldID fid,j##jtype##Array value) {\
	SetObjectArrayField(env,obj,fid,ubase_cast(jobjectArray,value));\
}\
JNIEXPORTC void JNICALL SetStatic##type##ArrayField(JNIEnv *env,jclass cls,jfieldID fid,j##jtype##Array value) {\
	SetStaticObjectArrayField(env,cls,fid,ubase_cast(jobjectArray,value));\
}

// ObjectArray
JNIEXPORTC jobjectArray JNICALL GetObjectArrayField(JNIEnv *env,jobject obj,jfieldID fid) {
    if (obj == NULL || fid == NULL) return NULL;
    return ubase_cast(jobjectArray,_GetObjectField(env,obj,fid));
}

JNIEXPORTC jobjectArray JNICALL GetStaticObjectArrayField(JNIEnv *env,jclass cls,jfieldID fid) {
	if (cls == NULL || fid == NULL) return NULL;
	return ubase_cast(jobjectArray,_GetStaticObjectField(env,cls,fid));
}

JNIEXPORTC void JNICALL SetObjectArrayField(JNIEnv *env,jobject obj,jfieldID fid,jobjectArray value) {
	if (obj == NULL || fid == NULL) return;
	_SetObjectField(env,obj,fid,ubase_cast(jobject,value));
}

JNIEXPORTC void JNICALL SetStaticObjectArrayField(JNIEnv *env,jclass cls,jfieldID fid,jobjectArray value) {
	if (cls == NULL || fid == NULL) return;
	_SetStaticObjectField(env,cls,fid,ubase_cast(jobject,value));
}

// Array
JNIEXPORTC jarray JNICALL GetArrayField(JNIEnv *env,jobject obj,jfieldID fid) {
	return ubase_cast(jarray,GetObjectArrayField(env,obj,fid));
}

JNIEXPORTC jarray JNICALL GetStaticArrayField(JNIEnv *env,jclass cls,jfieldID fid) {
	return ubase_cast(jarray,GetStaticObjectArrayField(env,cls,fid));
}

JNIEXPORTC void JNICALL SetArrayField(JNIEnv *env,jobject obj,jfieldID fid,jarray value) {
	SetObjectArrayField(env,obj,fid,ubase_cast(jobjectArray,value));
}

JNIEXPORTC void JNICALL SetStaticArrayField(JNIEnv *env,jclass cls,jfieldID fid,jarray value) {
	SetObjectArrayField(env,cls,fid,ubase_cast(jobjectArray,value));
}

DEFINE_ARRAY_FIELD_GETTERS_SETTERS(Byte,byte)
DEFINE_ARRAY_FIELD_GETTERS_SETTERS(Int,int)
DEFINE_ARRAY_FIELD_GETTERS_SETTERS(Long,long)
DEFINE_ARRAY_FIELD_GETTERS_SETTERS(Float,float)
DEFINE_ARRAY_FIELD_GETTERS_SETTERS(Double,double)
