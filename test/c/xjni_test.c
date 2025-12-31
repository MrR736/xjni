#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xjni.h"

JNIEXPORT void JNICALL
Java_TestXJNI_testStringUtilities(JNIEnv *env, jobject thiz, jcharArray input) {
    // Retrieve the char array from Java
    jchar* cin = (*env)->GetCharArrayElements(env, input, NULL);

    // Check if the array was retrieved successfully
    if (cin == NULL) {
        printf("Failed to retrieve char array elements\n");
        return; // Handle memory allocation failure
    }

    // Get the length of the char array
    size_t length = (*env)->GetArrayLength(env, input);

    // Example: Print the original string as a wide string (jchar array)
    printf("Original string: ");
    for (size_t i = 0; i < length; ++i) {
        printf("%lc", cin[i]);  // Print each character as a wide character
    }
    printf("\n");

    jstrreverse(cin);

    // Print the reversed string
    printf("Reversed string: ");
    for (size_t i = 0; i < length; ++i) {
        printf("%lc", cin[i]);  // Print each character of the reversed string
    }
    printf("\n");

    jstrreverse(cin);

    // Tokenize the reversed string (assuming jstrtok is defined)
    jchar* token = jstrtok(cin, u" ");
    printf("Tokenized string: ");
    while (token != NULL) {
        for (size_t i = 0; i < jstrlen(token); ++i) {
            printf("%lc", token[i]); // Print tokenized characters
        }
        token = jstrtok(NULL, u" ");
    }
    printf("\n");

    // Print the length of the string
    size_t str_len = jstrlen(cin);
    printf("Length of the string: %zu\n", str_len);

    // Release the char array (with changes, if any)
    (*env)->ReleaseCharArrayElements(env, input, cin, 0);  // Commit changes back to Java array
}

JNIEXPORT void Java_TestXJNI_testIOException(JNIEnv* env, jobject obj) {
    throwIOException(env, "xjni_test", "Stress IOException from C");
}

JNIEXPORT void Java_TestXJNI_testUTFException(JNIEnv* env, jobject obj) {
    throwUTFDataFormatException(env, "xjni_test", "Stress UTFDataFormatException from C");
}

JNIEXPORT void Java_TestXJNI_testFileNotFoundException(JNIEnv* env, jobject obj) {
    throwFileNotFoundException(env, "xjni_test", "Stress FileNotFoundException from C");
}

JNIEXPORT void Java_TestXJNI_testUnsupportedEncodingException(JNIEnv* env, jobject obj) {
    throwUnsupportedEncodingException(env, "xjni_test", "Stress UnsupportedEncodingException from C");
}

