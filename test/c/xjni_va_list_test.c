#include <xjni_va_list.h>

JNIEXPORT jstring JNICALL Java_TestXJNIPrintf_formatWithJNI
  (JNIEnv *env, jobject thiz, jstring format, jobjectArray args) {
    char buffer[4096];
    JSPrintf(env, buffer, format, args);
    return (*env)->NewStringUTF(env, buffer);
}
