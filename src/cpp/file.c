#include "base-jni.h"

#include <stdio.h>
#include <stdint.h>

BASE_EXTERN_C
static jfieldID getStreamHandleField(JNIEnv* env, jobject obj) {
	if (obj == NULL) return NULL;
	jclass cls = _GetObjectClass(env, obj);
	if (cls == NULL) return NULL;
	jfieldID field = _GetFieldID(env,cls,"streamHandle","J");
	_DeleteLocalRef(env, cls);
	return field;
}

BASE_EXTERN_C
static FILE* FILE_to_C(JNIEnv* env, jobject javaFile) {
	if (javaFile == NULL) return NULL;
	jfieldID field = getStreamHandleField(env,javaFile);
	if (field == NULL) return NULL;
	jlong handle = _GetLongField(env,javaFile,field);
	if (handle == 0) return NULL;
	return ubase_cast(FILE*,base_cast(uintptr_t, handle));
}

BASE_EXTERN_C
static void C_to_FILE(JNIEnv* env,jobject javaFile,FILE* fp) {
	if (javaFile == NULL) return;
	jfieldID field = getStreamHandleField(env,javaFile);
	if (field == NULL) return;
	jlong handle = base_cast(jlong,ubase_cast(uintptr_t, fp));
	_SetLongField(env,javaFile,field,handle);
}

JNIEXPORTC void JNICALL
Java_xjava_io_FILE_open(JNIEnv* env,jobject self,jobject file,jstring mode) {
	if (file == NULL || mode == NULL) return;
	const char* modeChars = _GetStringUTFChars(env, mode, NULL);
	if (modeChars == NULL) return;
	jclass fileClass = _FindClass(env, "java/io/File");
	if (fileClass == NULL) {
		_ReleaseStringUTFChars(env,mode,modeChars);
		return;
	}
	jmethodID getPath = _GetMethodID(env,fileClass,"getPath","()Ljava/lang/String;");
	if (getPath == NULL) {
		_DeleteLocalRef(env, fileClass);
		_ReleaseStringUTFChars(env,mode,modeChars);
		return;
	}
	jobject pathObj = _CallObjectMethod(env,file,getPath);
	if (pathObj == NULL) {
		_DeleteLocalRef(env, fileClass);
		_ReleaseStringUTFChars(env,mode,modeChars);
		return;
	}
	const char* path = _GetStringUTFChars(env,base_cast(jstring, pathObj),NULL);
	if (path == NULL) {
		_DeleteLocalRef(env, pathObj);
		_DeleteLocalRef(env, fileClass);
		_ReleaseStringUTFChars(env,mode,modeChars);
		return;
	}
	FILE* fp = fopen(path, modeChars);
	C_to_FILE(env,file,fp);
	_ReleaseStringUTFChars(env,base_cast(jstring, pathObj),path);
	_DeleteLocalRef(env, pathObj);
	_DeleteLocalRef(env, fileClass);
	_ReleaseStringUTFChars(env,mode,modeChars);
}

JNIEXPORTC void JNICALL
Java_xjava_io_FILE_close(JNIEnv* env,jobject self,jobject file) {
	if (file == NULL) return;
	FILE* fp = FILE_to_C(env,file);
	if (fp != NULL) {
		fclose(fp);
	}
	C_to_FILE(env,file,NULL);
}

JNIEXPORT jboolean JNICALL
Java_xjava_io_FILE_isOpen(JNIEnv* env,jobject self,jobject file) {
	return FILE_to_C(env, file) != NULL ? JNI_TRUE : JNI_FALSE;
}
