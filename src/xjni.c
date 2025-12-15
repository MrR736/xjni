#include <stdio.h>
#include <string.h>
#include <jni.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

const char* xjni_version(void) {
	char s[10];
	sprintf(s,"%d.%d.%d",_XJNI_VERSION_MAJOR,_XJNI_VERSION_MINOR,_XJNI_VERSION_PATCH);
	return strdup(s);
}

JNIEXPORTC void throwJava(JNIEnv *env,const char* tag,const char* msg,const char* cls) {
	jclass clas = _FindClass(env,cls);
	if (clas == NULL) {
# ifdef __ANDROID__
		__android_log_print(ANDROID_LOG_ERROR,tag,"Unable to find exception class: %s\n",cls);
#else
		fprintf(stderr,"[%s] ERROR: Unable to find exception class: %s\n",tag,cls);
#endif
		_ExceptionClear(env);
	} else {
		_ThrowNew(env,clas,msg);
		_DeleteLocalRef(env,clas);
	}
}

JNIEXPORTC void throwIOException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/IOException");
}

JNIEXPORTC void throwCharConversionException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/CharConversionException");
}

JNIEXPORTC void throwEOFException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/EOFException");
}

JNIEXPORTC void throwFileNotFoundException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/FileNotFoundException");
}

JNIEXPORTC void throwInterruptedIOException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/InterruptedIOException");
}

JNIEXPORTC void throwInvalidClassException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/InvalidClassException");
}

JNIEXPORTC void throwInvalidObjectException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/InvalidObjectException");
}

JNIEXPORTC void throwNotActiveException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/NotActiveException");
}

JNIEXPORTC void throwNotSerializableException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/NotSerializableException");
}

JNIEXPORTC void throwOptionalDataException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/OptionalDataException");
}

JNIEXPORTC void throwStreamCorruptedException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/StreamCorruptedException");
}

JNIEXPORTC void throwSyncFailedException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/SyncFailedException");
}

JNIEXPORTC void throwUTFDataFormatException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/UTFDataFormatException");
}

JNIEXPORTC void throwUnsupportedEncodingException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/UnsupportedEncodingException");
}

JNIEXPORTC void throwWriteAbortedException(JNIEnv *env,const char* tag,const char* msg) {
	throwJava(env,tag,msg,"java/io/WriteAbortedException");
}
