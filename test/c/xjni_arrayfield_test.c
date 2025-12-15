#include <jni.h>
#include <stdio.h>
#include <xjni_arrayfield.h>

/* ===== native test ===== */

JNIEXPORT void JNICALL Java_ArrayFieldTest_nativeTest(JNIEnv *env, jobject obj) {

    jclass cls = (*env)->GetObjectClass(env, obj);

    jfieldID fidStr =
        (*env)->GetFieldID(env, cls, "stringArray", "[Ljava/lang/String;");
    jfieldID fidInt =
        (*env)->GetFieldID(env, cls, "intArray", "[I");

    jfieldID fidStrS =
        (*env)->GetStaticFieldID(env, cls, "staticStringArray", "[Ljava/lang/String;");
    jfieldID fidIntS =
        (*env)->GetStaticFieldID(env, cls, "staticIntArray", "[I");

    jobjectArray strArr = GetObjectArrayField(env, obj, fidStr);
    jintArray intArr = GetIntArrayField(env, obj, fidInt);

    jobjectArray sStrArr = GetStaticObjectArrayField(env, cls, fidStrS);
    jintArray sIntArr = GetStaticIntArrayField(env, cls, fidIntS);

    // mutate instance String[]
    jstring extra = (*env)->NewStringUTF(env, "JNI");
    (*env)->SetObjectArrayElement(env, strArr, 0, extra);

    // mutate instance int[]
    jint *ints = (*env)->GetIntArrayElements(env, intArr, NULL);
    ints[0] = 99;
    (*env)->ReleaseIntArrayElements(env, intArr, ints, 0);

    // mutate static String[]
    jstring extra2 = (*env)->NewStringUTF(env, "STATIC_JNI");
    (*env)->SetObjectArrayElement(env, sStrArr, 1, extra2);

    // mutate static int[]
    jint *sints = (*env)->GetIntArrayElements(env, sIntArr, NULL);
    sints[1] = 777;
    (*env)->ReleaseIntArrayElements(env, sIntArr, sints, 0);
}
