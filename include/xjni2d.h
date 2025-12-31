/**
 * xjni2d.h: Extern JNI 2D Array Utility
 *
 * Copyright (C) 2025 MrR736 <MrR736@users.github.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * The complete text of the GNU General Public License
 * can be found in /usr/share/common-licenses/GPL-3 file.
 */

#ifndef __XJNI2D_H__
#define __XJNI2D_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

// Byte2D - Access and release functions for Java byte[][]

/**
 * Retrieves a native 2D pointer view of a Java byte[][]
 *
 * @param env      JNI environment pointer
 * @param array    Java 2D byte array (byte[][])
 * @param isCopy   Optional pointer to a jboolean to indicate if a copy was made
 *
 * @return         A jbyte** (array of pointers to jbyte arrays), or NULL on failure
 *                 Must be released with ReleaseByte2DArrayElements()
 */
JNIEXPORT jbyte** JNICALL GetByte2DArrayElements(JNIEnv *env, jobjectArray array,jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via GetByte2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java byte[][]
 * @param elements The jbyte** array returned by GetByte2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT void JNICALL ReleaseByte2DArrayElements(JNIEnv *env, jobjectArray array,jbyte **elements, jint mode);

/**
 * Copies data from a native 2D buffer into a Java byte[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java byte[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of byte array pointers)
 */
JNIEXPORT void JNICALL SetByte2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, const jbyte **buf);

/**
 * Copies data from a Java int[][] array into a native 2D buffer
 *
 * @param env      JNI environment pointer
 * @param array    The source Java int[][] array
 * @param start    Start index in outer array to begin reading (typically 0)
 * @param len      Number of rows to read
 * @param buf      The native buffer (array of int array pointers)
 *                 Each buf[i] must be pre-allocated with at least the size
 *                 of the corresponding inner array length.
 */
JNIEXPORT void JNICALL GetByte2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, jbyte **buf);

// Int2D - Access and release functions for Java int[][]

/**
 * Retrieves a native 2D pointer view of a Java int[][]
 *
 * @param env      JNI environment pointer
 * @param array    Java 2D int array (int[][])
 * @param isCopy   Optional pointer to a jboolean to indicate if a copy was made
 *
 * @return         A jint** (array of pointers to jint arrays), or NULL on failure
 *                 Must be released with ReleaseInt2DArrayElements()
 */
JNIEXPORT jint** JNICALL GetInt2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseInt2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java int[][]
 * @param elements The jint** array returned by ReleaseInt2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT void JNICALL ReleaseInt2DArrayElements(JNIEnv *env, jobjectArray array, jint **elements,jint mode);

/**
 * Copies data from a native 2D buffer into a Java int[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java int[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of int array pointers)
 */
JNIEXPORT void JNICALL SetInt2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, const jint **buf);

/**
 * Copies data from a Java int[][] array into a native 2D buffer
 *
 * @param env      JNI environment pointer
 * @param array    The source Java int[][] array
 * @param start    Start index in outer array to begin reading (typically 0)
 * @param len      Number of rows to read
 * @param buf      The native buffer (array of int array pointers)
 *                 Each buf[i] must be pre-allocated with at least the size
 *                 of the corresponding inner array length.
 */
JNIEXPORT void JNICALL GetInt2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, jint **buf);

// Long2D - Access and release functions for Java long[][]

/**
 * Releases the native 2D pointer view obtained via GetLong2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java long[][]
 * @param elements The jlong** array returned by GetLong2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT jlong** JNICALL GetLong2DArrayElements(JNIEnv *env, jobjectArray array,jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseLong2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java int[][]
 * @param elements The jlong** array returned by ReleaseLong2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT void JNICALL ReleaseLong2DArrayElements(JNIEnv *env, jobjectArray array,jlong **elements, jint mode);

/**
 * Copies data from a native 2D buffer into a Java long[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java long[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of long array pointers)
 */
JNIEXPORT void JNICALL SetLong2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, const jlong **buf);

/**
 * Copies data from a Java long[][] array into a native 2D buffer
 *
 * @param env      JNI environment pointer
 * @param array    The source Java long[][] array
 * @param start    Start index in outer array to begin reading (typically 0)
 * @param len      Number of rows to read
 * @param buf      The native buffer (array of long array pointers)
 *                 Each buf[i] must be pre-allocated with at least the size
 *                 of the corresponding inner array length.
 */
JNIEXPORT void JNICALL GetLong2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, jlong **buf);

// Float2D - Access and release functions for Java float[][]

/**
 * Releases the native 2D pointer view obtained via GetFloat2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java float[][]
 * @param elements The jfloat** array returned by GetFloat2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT jfloat** JNICALL GetFloat2DArrayElements(JNIEnv *env, jobjectArray array,jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseFloat2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java int[][]
 * @param elements The jfloat** array returned by ReleaseFloat2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT void JNICALL ReleaseFloat2DArrayElements(JNIEnv *env, jobjectArray array,jfloat **elements, jint mode);

/**
 * Copies data from a native 2D buffer into a Java float[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java float[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of float array pointers)
 */
JNIEXPORT void JNICALL SetFloat2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, const jfloat **buf);

/**
 * Copies data from a Java float[][] array into a native 2D buffer
 *
 * @param env      JNI environment pointer
 * @param array    The source Java float[][] array
 * @param start    Start index in outer array to begin reading (typically 0)
 * @param len      Number of rows to read
 * @param buf      The native buffer (array of float array pointers)
 *                 Each buf[i] must be pre-allocated with at least the size
 *                 of the corresponding inner array length.
 */
JNIEXPORT void JNICALL GetFloat2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, jfloat **buf);

// Double2D - Access and release functions for Java double[][].

/**
 * Releases the native 2D pointer view obtained via GetDouble2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java double[][]
 * @param elements The jdouble** array returned by GetDouble2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT jdouble** JNICALL GetDouble2DArrayElements(JNIEnv *env, jobjectArray array,jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseDouble2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java int[][]
 * @param elements The jdouble** array returned by ReleaseDouble2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT void JNICALL ReleaseDouble2DArrayElements(JNIEnv *env, jobjectArray array,jdouble **elements, jint mode);

/**
 * Copies data from a native 2D buffer into a Java double[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java double[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of double array pointers)
 */
JNIEXPORT void JNICALL SetDouble2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, const jdouble **buf);

/**
 * Copies data from a Java double[][] array into a native 2D buffer
 *
 * @param env      JNI environment pointer
 * @param array    The source Java double[][] array
 * @param start    Start index in outer array to begin reading (typically 0)
 * @param len      Number of rows to read
 * @param buf      The native buffer (array of double array pointers)
 *                 Each buf[i] must be pre-allocated with at least the size
 *                 of the corresponding inner array length.
 */
JNIEXPORT void JNICALL GetDouble2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, jdouble **buf);

// Short2D - Access and release functions for Java short[][].

/**
 * Releases the native 2D pointer view obtained via GetShort2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java short[][]
 * @param elements The jshort** array returned by GetShort2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT jshort** JNICALL GetShort2DArrayElements(JNIEnv *env, jobjectArray array,jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseShort2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java short[][]
 * @param elements The jshort** array returned by ReleaseShort2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT void JNICALL ReleaseShort2DArrayElements(JNIEnv *env, jobjectArray array,jshort **elements, jint mode);

/**
 * Copies data from a native 2D buffer into a Java short[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java short[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of short array pointers)
 */
JNIEXPORT void JNICALL SetShort2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, const jshort **buf);

/**
 * Copies data from a Java short[][] array into a native 2D buffer
 *
 * @param env      JNI environment pointer
 * @param array    The source Java short[][] array
 * @param start    Start index in outer array to begin reading (typically 0)
 * @param len      Number of rows to read
 * @param buf      The native buffer (array of short array pointers)
 *                 Each buf[i] must be pre-allocated with at least the size
 *                 of the corresponding inner array length.
 */
JNIEXPORT void JNICALL GetShort2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, jshort **buf);


// Char2D - Access and release functions for Java char[][].

/**
 * Releases the native 2D pointer view obtained via GetChar2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java char[][]
 * @param elements The jchar** array returned by GetChar2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT jchar** JNICALL GetChar2DArrayElements(JNIEnv *env, jobjectArray array,jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseChar2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java char[][]
 * @param elements The jchar** array returned by ReleaseChar2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT void JNICALL ReleaseChar2DArrayElements(JNIEnv *env, jobjectArray array,jchar **elements, jint mode);

/**
 * Copies data from a native 2D buffer into a Java char[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java char[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of char array pointers)
 */
JNIEXPORT void JNICALL SetChar2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, const jchar **buf);

/**
 * Copies data from a Java char[][] array into a native 2D buffer
 *
 * @param env      JNI environment pointer
 * @param array    The source Java char[][] array
 * @param start    Start index in outer array to begin reading (typically 0)
 * @param len      Number of rows to read
 * @param buf      The native buffer (array of char array pointers)
 *                 Each buf[i] must be pre-allocated with at least the size
 *                 of the corresponding inner array length.
 */
JNIEXPORT void JNICALL GetChar2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, jchar **buf);


// StringUTF2D - Access and release functions for Java String[][].

/**
 * Releases the native 2D pointer view obtained via GetStringUTF2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java String[][]
 * @param elements The const char*** array returned by GetStringUTF2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT const char*** JNICALL GetStringUTF2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseStringUTF2DArrayChars
 *
 * @param env      JNI environment pointer
 * @param array    The original Java String[][]
 * @param elements The const char*** array returned by ReleaseStringUTF2DArrayChars
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT void JNICALL ReleaseStringUTF2DArrayChars(JNIEnv *env, jobjectArray array, const char ***elements, jint mode);

/**
 * Copies data from a native 2D buffer into a Java String[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java String[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of String array pointers)
 */
JNIEXPORT void JNICALL SetStringUTF2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const char ***buf);

/**
 * Copies data from a native 2D buffer into a Java String[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java String[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of String array pointers)
 */
JNIEXPORT void JNICALL GetStringUTF2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, char ***buf);

// String2D - Access and release functions for Java String[][].

/**
 * Releases the native 2D pointer view obtained via GetString2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java String[][]
 * @param elements The const jchar*** array returned by GetString2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT const jchar*** JNICALL GetString2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseString2DArrayChars
 *
 * @param env      JNI environment pointer
 * @param array    The original Java String[][]
 * @param elements The const jchar*** array returned by ReleaseString2DArrayChars
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
JNIEXPORT void JNICALL ReleaseString2DArrayChars(JNIEnv *env, jobjectArray array, const jchar ***elements);

/**
 * Copies data from a native 2D buffer into a Java String[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java String[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of String array pointers)
 */
JNIEXPORT void JNICALL SetString2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jchar ***buf);

/**
 * Copies data from a native 2D buffer into a Java String[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java String[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of String array pointers)
 */
JNIEXPORT void JNICALL GetString2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jchar ***buf);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI2D_H__ */
