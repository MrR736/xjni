#include "xjni_args.h"
#include <xjni_va_list.h>

JNIEXPORT jstring JNICALL Java_TestXJNIPrintf_formatWithJNI
  (JNIEnv *env, jobject thiz, jstring format, jargs_t args) {
    char buffer[4096];
    JSPrintf(env, buffer, format, args);
    return (*env)->NewStringUTF(env, buffer);
}

JNIEXPORT jstring JNICALL Java_TestXJNIPrintf_formatWithJNINoArgs
  (JNIEnv *env, jobject thiz) {

    jargs_t args = NewJArgs(env, 10, (*env)->FindClass(env,"java/lang/Object"), NULL);
    if (!args) return NULL;

    JArgsAppendChar(env, args, 'A');         // index 0
    JArgsInsertDouble(env, args, 3.14, 1);   // index 1
    JArgsAppendInt(env, args, 42);           // index 2
    jstring helloStr = (*env)->NewStringUTF(env, "hello");
    JArgsAppendString(env, args, helloStr);  // index 3
    JArgsAppendBoolean(env, args, JNI_TRUE); // index 4
    JArgsReplaceInt(env, args,(jchar)0x754C,0);      // Replace index 0

    // keep helloStr until after formatting
    char buffer[4096];
    JSnPrintfUTF(env, buffer, sizeof(buffer), "Char: %c, Double: %f, Int: %d, String: %s, Boolean: %b", args);
    (*env)->DeleteLocalRef(env, helloStr);
    (*env)->DeleteLocalRef(env, args);
    return (*env)->NewStringUTF(env, buffer);
}
