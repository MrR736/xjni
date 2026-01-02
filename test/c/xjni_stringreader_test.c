#include <jni.h>
#include <xjni_stringreader.h>

/* void ensureOpen() */
JNIEXPORT void JNICALL
Java_TestStringReader_StringReaderEnsureOpen(
        JNIEnv *env,
        jobject obj,
        jstringReader sr)
{
    StringReaderEnsureOpen(env, sr);
}

/* int read() */
JNIEXPORT jint JNICALL
Java_TestStringReader_StringReaderRead(
        JNIEnv *env,
        jobject obj,
        jstringReader sr)
{
    return StringReaderRead(env, sr);
}

/* int read(char[] buf, int offset, int len) */
JNIEXPORT jint JNICALL
Java_TestStringReader_StringReaderReadCharArrayIntInt(
        JNIEnv *env,
        jobject obj,
        jstringReader sr,
        jcharArray array,
        jint offset,
        jint len)
{
    return StringReaderReadCharArrayIntInt(env, sr, array, offset, len);
}

/* long skip(long n) */
JNIEXPORT jlong JNICALL
Java_TestStringReader_StringReaderReadSkip(
        JNIEnv *env,
        jobject obj,
        jstringReader sr,
        jlong ns)
{
    return StringReaderReadSkip(env, sr, ns);
}

/* boolean ready() */
JNIEXPORT jboolean JNICALL
Java_TestStringReader_StringReaderReadReady(
        JNIEnv *env,
        jobject obj,
        jstringReader sr)
{
    return StringReaderReadReady(env, sr);
}

/* boolean markSupported() */
JNIEXPORT jboolean JNICALL
Java_TestStringReader_StringReaderReadMarkSupported(
        JNIEnv *env,
        jobject obj,
        jstringReader sr)
{
    return StringReaderReadMarkSupported(env, sr);
}

/* void mark(int readAheadLimit) */
JNIEXPORT void JNICALL
Java_TestStringReader_StringReaderMark(
        JNIEnv *env,
        jobject obj,
        jstringReader sr,
        jint readAheadLimit)
{
    StringReaderMark(env, sr, readAheadLimit);
}

/* void reset() */
JNIEXPORT void JNICALL
Java_TestStringReader_StringReaderReset(
        JNIEnv *env,
        jobject obj,
        jstringReader sr)
{
    StringReaderReset(env, sr);
}

/* void close() */
JNIEXPORT void JNICALL
Java_TestStringReader_StringReaderClose(
        JNIEnv *env,
        jobject obj,
        jstringReader sr)
{
    StringReaderClose(env, sr);
}
