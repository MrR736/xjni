#include "xjni_args.h"
#include <xjni_va_list.h>

JNIEXPORT jstring JNICALL Java_TestXJNIPrintf_formatWithJNI(JNIEnv *env, jobject thiz, jstring format, jargs_t args) {
	char buffer[4096];
	JSPrintf(env, buffer, format, args);
	return (*env)->NewStringUTF(env, buffer);
}

JNIEXPORT jstring JNICALL Java_TestXJNIPrintf_formatWithJNINoArgs(JNIEnv *env, jobject thiz) {
	jstring helloStr = (*env)->NewStringUTF(env, "hello");
	jargs_t args = JArgsStart(env,5,"CDILjava/lang/String;Z",'A', 3.14, 42, helloStr, JNI_TRUE);
	if (!args) return NULL;
	JArgsReplaceChar(env, args, (jchar)0x754C, 0);
	char buffer[4096];
	JSnPrintfUTF(env, buffer, sizeof(buffer),"Char: %c, Double: %.3g, Int: %d, String: %s, Boolean: %b",args);
	(*env)->DeleteLocalRef(env, helloStr);
	(*env)->DeleteLocalRef(env, args);
	return (*env)->NewStringUTF(env, buffer);
}

