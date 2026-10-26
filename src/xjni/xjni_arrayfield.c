#include "base-jni.h"

#include <xjnidef.h>
#include <xjni_arrayfield.h>

// ObjectArray
XJNIAPI jobjectArray XJNICALL GetObjectArrayField(JNIEnv *env,jobject obj,jfieldID fid) {
    if (obj == NULL || fid == NULL) return NULL;
    return ubase_cast(jobjectArray,_GetObjectField(env,obj,fid));
}

XJNIAPI jobjectArray XJNICALL GetStaticObjectArrayField(JNIEnv *env,jclass cls,jfieldID fid) {
	if (cls == NULL || fid == NULL) return NULL;
	return ubase_cast(jobjectArray,_GetStaticObjectField(env,cls,fid));
}

XJNIAPI void XJNICALL SetObjectArrayField(JNIEnv *env,jobject obj,jfieldID fid,jobjectArray value) {
	if (obj == NULL || fid == NULL) return;
	_SetObjectField(env,obj,fid,ubase_cast(jobject,value));
}

XJNIAPI void XJNICALL SetStaticObjectArrayField(JNIEnv *env,jclass cls,jfieldID fid,jobjectArray value) {
	if (cls == NULL || fid == NULL) return;
	_SetStaticObjectField(env,cls,fid,ubase_cast(jobject,value));
}

// Array
XJNIAPI jarray XJNICALL GetArrayField(JNIEnv *env,jobject obj,jfieldID fid) {
	return ubase_cast(jarray,GetObjectArrayField(env,obj,fid));
}

XJNIAPI jarray XJNICALL GetStaticArrayField(JNIEnv *env,jclass cls,jfieldID fid) {
	return ubase_cast(jarray,GetStaticObjectArrayField(env,cls,fid));
}

XJNIAPI void XJNICALL SetArrayField(JNIEnv *env,jobject obj,jfieldID fid,jarray value) {
	SetObjectArrayField(env,obj,fid,ubase_cast(jobjectArray,value));
}

XJNIAPI void XJNICALL SetStaticArrayField(JNIEnv *env,jclass cls,jfieldID fid,jarray value) {
	SetObjectArrayField(env,cls,fid,ubase_cast(jobjectArray,value));
}

#define DEFINE_ARRAY_FIELD_GETTERS_SETTERS(type,jtype) \
XJNIAPI j##jtype##Array XJNICALL Get##type##ArrayField(JNIEnv *env,jobject obj,jfieldID fid) { \
	return ubase_cast(j##jtype##Array,GetObjectArrayField(env,obj,fid)); \
} \
XJNIAPI j##jtype##Array XJNICALL GetStatic##type##ArrayField(JNIEnv *env,jclass cls,jfieldID fid) { \
	return ubase_cast(j##jtype##Array,GetStaticObjectArrayField(env,cls,fid)); \
}\
XJNIAPI void XJNICALL Set##type##ArrayField(JNIEnv *env,jobject obj,jfieldID fid,j##jtype##Array value) {\
	SetObjectArrayField(env,obj,fid,ubase_cast(jobjectArray,value));\
}\
XJNIAPI void XJNICALL SetStatic##type##ArrayField(JNIEnv *env,jclass cls,jfieldID fid,j##jtype##Array value) {\
	SetStaticObjectArrayField(env,cls,fid,ubase_cast(jobjectArray,value));\
}

DEFINE_ARRAY_FIELD_GETTERS_SETTERS(Byte,byte)
DEFINE_ARRAY_FIELD_GETTERS_SETTERS(Int,int)
DEFINE_ARRAY_FIELD_GETTERS_SETTERS(Long,long)
DEFINE_ARRAY_FIELD_GETTERS_SETTERS(Float,float)
DEFINE_ARRAY_FIELD_GETTERS_SETTERS(Double,double)
DEFINE_ARRAY_FIELD_GETTERS_SETTERS(Boolean,boolean)
