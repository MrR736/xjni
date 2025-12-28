#include <jni.h>
#include <stdio.h>
#include "xjni.h"

JNIEXPORT void testIOException(JNIEnv* env, jobject obj) {
    throwIOException(env, "xjni_test", "Stress IOException from C");
}

JNIEXPORT void testUTFException(JNIEnv* env, jobject obj) {
    throwUTFDataFormatException(env, "xjni_test", "Stress UTFDataFormatException from C");
}

JNIEXPORT void testFileNotFoundException(JNIEnv* env, jobject obj) {
    throwFileNotFoundException(env, "xjni_test", "Stress FileNotFoundException from C");
}

JNIEXPORT void testUnsupportedEncodingException(JNIEnv* env, jobject obj) {
    throwUnsupportedEncodingException(env, "xjni_test", "Stress UnsupportedEncodingException from C");
}

