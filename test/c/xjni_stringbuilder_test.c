#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <xjni_stringbuilder.h>

/* =========================
 * Native test
 * ========================= */

JNIEXPORT jstringBuilder JNICALL
Java_TestStringBuilder_NewStringBuilder(JNIEnv *env, jobject obj) {
	return NewStringBuilder(env);
}

JNIEXPORT void JNICALL
Java_TestStringBuilder_StringBuilderAppendString(JNIEnv *env, jobject obj,jstringBuilder sb, jstring str) {
	StringBuilderAppendString(env,sb,str);
}

JNIEXPORT void JNICALL
Java_TestStringBuilder_StringBuilderAppendInt(JNIEnv *env, jobject obj, jstringBuilder sb, jint num) {
	StringBuilderAppendInt(env,sb,num);
}

JNIEXPORT void JNICALL
Java_TestStringBuilder_StringBuilderInsertString(JNIEnv *env, jobject obj, jstringBuilder sb, jint offset, jstring str) {
	StringBuilderInsertString(env,sb,offset,str);
}

JNIEXPORT jint JNICALL Java_TestStringBuilder_StringBuilderIndexOf(JNIEnv *env, jobject obj, jstringBuilder sb, jstring str) {
	return StringBuilderIndexOf(env,sb,str);
}

JNIEXPORT void JNICALL Java_TestStringBuilder_StringBuilderReverse(JNIEnv *env, jobject obj, jstringBuilder sb) {
	StringBuilderReverse(env,sb);
}
