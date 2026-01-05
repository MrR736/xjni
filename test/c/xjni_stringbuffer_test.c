#include <stdlib.h>
#include <xjni_stringbuffer.h>

JNIEXPORT jstringBuffer JNICALL Java_TestStringBuffer_NewStringBuffer(JNIEnv *env, jobject obj) {
	return NewStringBuffer(env);
}

JNIEXPORT jstringBuffer JNICALL Java_TestStringBuffer_NewStringBufferCapacity(JNIEnv *env, jobject obj, jint str) {
	return NewStringBufferCapacity(env, str);
}

JNIEXPORT jstringBuffer JNICALL Java_TestStringBuffer_NewStringBufferString(JNIEnv *env, jobject obj, jstring str) {
	return NewStringBufferString(env, str);
}

JNIEXPORT void JNICALL Java_TestStringBuffer_StringBufferAppendString(JNIEnv *env, jobject obj, jstringBuffer sb, jstring str) {
	StringBufferAppendString(env, sb, str);
}

JNIEXPORT jstring JNICALL Java_TestStringBuffer_StringBufferToString(JNIEnv *env, jobject obj, jstringBuffer sb) {
	return StringBufferToString(env, sb);
}

JNIEXPORT jint JNICALL Java_TestStringBuffer_StringBufferLength(JNIEnv *env, jobject obj, jstringBuffer sb) {
	return StringBufferLength(env, sb);
}

JNIEXPORT void JNICALL Java_TestStringBuffer_StringBufferReverse(JNIEnv *env, jobject obj, jstringBuffer sb) {
	StringBufferReverse(env, sb);
}
