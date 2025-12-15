#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <xjni2d.h>

/* =========================
 * Native test
 * ========================= */

JNIEXPORT void JNICALL
Java_Array2DTest_nativeTest(JNIEnv *env, jobject obj) {

    jclass cls = (*env)->GetObjectClass(env, obj);

    jfieldID fidInt =
        (*env)->GetFieldID(env, cls, "int2d", "[[I");
    jfieldID fidStr =
        (*env)->GetFieldID(env, cls, "str2d", "[[Ljava/lang/String;");

    jobjectArray intArr =
        (jobjectArray)(*env)->GetObjectField(env, obj, fidInt);
    jobjectArray strArr =
        (jobjectArray)(*env)->GetObjectField(env, obj, fidStr);

    // ---- int[][]
    jint **ints = GetInt2DArrayElements(env, intArr, NULL);
    ints[0][0] = 42;
    ints[1][2] = 99;
    ReleaseInt2DArrayElements(env, intArr, ints, 0);

    // ---- String[][]
    const char ***strs = GetStringUTF2DArrayElements(env, strArr, NULL);

    jstring repl = (*env)->NewStringUTF(env, "JNI");
    jobjectArray row0 =
        (jobjectArray)(*env)->GetObjectArrayElement(env, strArr, 0);
    (*env)->SetObjectArrayElement(env, row0, 1, repl);
    (*env)->DeleteLocalRef(env, row0);
    (*env)->DeleteLocalRef(env, repl);

    ReleaseStringUTF2DArrayChars(env, strArr, strs, 0);
}
