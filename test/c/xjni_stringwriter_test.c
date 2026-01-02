#include <jni.h>
#include <xjni_stringwriter.h>

JNIEXPORT void JNICALL
Java_TestStringWriter_StringWriterWriteCharArrayIntInt(
        JNIEnv *env,
        jobject obj,
        jstringWriter sw,
        jcharArray array,
        jint offset,
        jint len) {
    StringWriterWriteCharArrayIntInt(env, sw, array, offset, len);
}

JNIEXPORT void JNICALL
Java_TestStringWriter_StringWriterWriteInt(
        JNIEnv *env,
        jobject obj,
        jstringWriter sw,
        jint value) {
    StringWriterWriteInt(env, sw, value);
}

JNIEXPORT void JNICALL
Java_TestStringWriter_StringWriterWriteString(
        JNIEnv *env,
        jobject obj,
        jstringWriter sw,
        jstring str) {
    StringWriterWriteString(env, sw, str);
}

JNIEXPORT void JNICALL
Java_TestStringWriter_StringWriterWriteStringIntInt(
        JNIEnv *env,
        jobject obj,
        jstringWriter sw,
        jstring str,
        jint offset,
        jint len) {
    StringWriterWriteStringIntInt(env, sw, str, offset, len);
}

JNIEXPORT void JNICALL
Java_TestStringWriter_StringWriterFlush(
        JNIEnv *env,
        jobject obj,
        jstringWriter sw) {
    StringWriterFlush(env, sw);
}

JNIEXPORT void JNICALL
Java_TestStringWriter_StringWriterAppendChar(
        JNIEnv *env,
        jobject obj,
        jstringWriter sw,
        jchar ch) {
    StringWriterAppendChar(env, sw, ch);
}
