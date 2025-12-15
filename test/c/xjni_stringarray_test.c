#include <xjni_stringarray.h>
#include <stdlib.h>
#include <string.h>

JNIEXPORT jobjectArray JNICALL
Java_TestStringArray_createStringArray(JNIEnv *env, jobject thiz, jobjectArray input) {
    jsize len = GetStringUTFArrayLength(env, input);

    const char **elements = GetStringUTFArrayChars(env, input, NULL);

    // Create a new array with "_native" appended to each string
    char **newStrings = (char **)malloc(sizeof(char *) * len);
    for (jsize i = 0; i < len; i++) {
        size_t l = strlen(elements[i]) + 8;
        newStrings[i] = (char *)malloc(l);
        snprintf(newStrings[i], l, "%s_native", elements[i]);
    }

    jobjectArray result = NewStringUTFArray(env, (const char **)newStrings, len);

    // Free temporary allocations
    for (jsize i = 0; i < len; i++) free(newStrings[i]);
    free(newStrings);
    ReleaseStringUTFArrayChars(env, input, elements, 0);

    return result;
}

JNIEXPORT void JNICALL
Java_TestStringArray_printStringArray(JNIEnv *env, jobject thiz, jobjectArray arr) {
    jsize len = GetStringUTFArrayLength(env, arr);
    const char **elements = GetStringUTFArrayChars(env, arr, NULL);

    for (jsize i = 0; i < len; i++) {
        printf("[%d]: %s\n", i, elements[i]);
    }

    ReleaseStringUTFArrayChars(env, arr, elements, 0);
}
