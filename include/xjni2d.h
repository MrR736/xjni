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
extern jbyte **GetByte2DArrayElements(JNIEnv *env, jobjectArray array,jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via GetByte2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java byte[][]
 * @param elements The jbyte** array returned by GetByte2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern void ReleaseByte2DArrayElements(JNIEnv *env, jobjectArray array,jbyte **elements, jint mode);

/**
 * Copies data from a native 2D buffer into a Java byte[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java byte[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of byte array pointers)
 */
extern void SetByte2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, const jbyte **buf);

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
extern void GetByte2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, jbyte **buf);

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
extern jint **GetInt2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseInt2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java int[][]
 * @param elements The jint** array returned by ReleaseInt2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern void ReleaseInt2DArrayElements(JNIEnv *env, jobjectArray array, jint **elements,jint mode);

/**
 * Copies data from a native 2D buffer into a Java int[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java int[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of int array pointers)
 */
extern void SetInt2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, const jint **buf);

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
extern void GetInt2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, jint **buf);

// Long2D - Access and release functions for Java long[][]

/**
 * Releases the native 2D pointer view obtained via GetLong2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java long[][]
 * @param elements The jlong** array returned by GetLong2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern jlong **GetLong2DArrayElements(JNIEnv *env, jobjectArray array,jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseLong2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java int[][]
 * @param elements The jlong** array returned by ReleaseLong2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern void ReleaseLong2DArrayElements(JNIEnv *env, jobjectArray array,jlong **elements, jint mode);

/**
 * Copies data from a native 2D buffer into a Java long[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java long[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of long array pointers)
 */
extern void SetLong2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, const jlong **buf);

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
extern void GetLong2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, jlong **buf);

// Float2D - Access and release functions for Java float[][]

/**
 * Releases the native 2D pointer view obtained via GetFloat2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java float[][]
 * @param elements The jfloat** array returned by GetFloat2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern jfloat **GetFloat2DArrayElements(JNIEnv *env, jobjectArray array,jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseFloat2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java int[][]
 * @param elements The jfloat** array returned by ReleaseFloat2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern void ReleaseFloat2DArrayElements(JNIEnv *env, jobjectArray array,jfloat **elements, jint mode);

/**
 * Copies data from a native 2D buffer into a Java float[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java float[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of float array pointers)
 */
extern void SetFloat2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, const jfloat **buf);

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
extern void GetFloat2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, jfloat **buf);

// Double2D - Access and release functions for Java double[][].

/**
 * Releases the native 2D pointer view obtained via GetDouble2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java double[][]
 * @param elements The jdouble** array returned by GetDouble2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern jdouble **GetDouble2DArrayElements(JNIEnv *env, jobjectArray array,jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseDouble2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java int[][]
 * @param elements The jdouble** array returned by ReleaseDouble2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern void ReleaseDouble2DArrayElements(JNIEnv *env, jobjectArray array,jdouble **elements, jint mode);

/**
 * Copies data from a native 2D buffer into a Java double[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java double[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of double array pointers)
 */
extern void SetDouble2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, const jdouble **buf);

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
extern void GetDouble2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, jdouble **buf);

// Short2D - Access and release functions for Java short[][].

/**
 * Releases the native 2D pointer view obtained via GetShort2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java short[][]
 * @param elements The jshort** array returned by GetShort2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern jshort **GetShort2DArrayElements(JNIEnv *env, jobjectArray array,jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseShort2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java short[][]
 * @param elements The jshort** array returned by ReleaseShort2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern void ReleaseShort2DArrayElements(JNIEnv *env, jobjectArray array,jshort **elements, jint mode);

/**
 * Copies data from a native 2D buffer into a Java short[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java short[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of short array pointers)
 */
extern void SetShort2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, const jshort **buf);

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
extern void GetShort2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, jshort **buf);


// Char2D - Access and release functions for Java char[][].

/**
 * Releases the native 2D pointer view obtained via GetChar2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java char[][]
 * @param elements The jchar** array returned by GetChar2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern jchar **GetChar2DArrayElements(JNIEnv *env, jobjectArray array,jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseChar2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java char[][]
 * @param elements The jchar** array returned by ReleaseChar2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern void ReleaseChar2DArrayElements(JNIEnv *env, jobjectArray array,jchar **elements, jint mode);

/**
 * Copies data from a native 2D buffer into a Java char[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java char[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of char array pointers)
 */
extern void SetChar2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, const jchar **buf);

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
extern void GetChar2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start,jsize len, jchar **buf);


// StringUTF2D - Access and release functions for Java String[][].

/**
 * Releases the native 2D pointer view obtained via GetStringUTF2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java String[][]
 * @param elements The const char*** array returned by GetStringUTF2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern const char ***GetStringUTF2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseStringUTF2DArrayChars
 *
 * @param env      JNI environment pointer
 * @param array    The original Java String[][]
 * @param elements The const char*** array returned by ReleaseStringUTF2DArrayChars
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern void ReleaseStringUTF2DArrayChars(JNIEnv *env, jobjectArray array, const char ***elements, jint mode);

/**
 * Copies data from a native 2D buffer into a Java String[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java String[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of String array pointers)
 */
extern void SetStringUTF2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const char ***buf);

/**
 * Copies data from a native 2D buffer into a Java String[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java String[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of String array pointers)
 */
extern void GetStringUTF2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, char ***buf);

// String2D - Access and release functions for Java String[][].

/**
 * Releases the native 2D pointer view obtained via GetString2DArrayElements
 *
 * @param env      JNI environment pointer
 * @param array    The original Java String[][]
 * @param elements The const jchar*** array returned by GetString2DArrayElements
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern const jchar ***GetString2DArrayElements(JNIEnv *env, jobjectArray array, jboolean *isCopy);

/**
 * Releases the native 2D pointer view obtained via ReleaseString2DArrayChars
 *
 * @param env      JNI environment pointer
 * @param array    The original Java String[][]
 * @param elements The const jchar*** array returned by ReleaseString2DArrayChars
 * @param mode     Release mode: 0 (copy back), JNI_COMMIT, or JNI_ABORT
 */
extern void ReleaseString2DArrayChars(JNIEnv *env, jobjectArray array, const jchar ***elements);

/**
 * Copies data from a native 2D buffer into a Java String[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java String[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of String array pointers)
 */
extern void SetString2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, const jchar ***buf);

/**
 * Copies data from a native 2D buffer into a Java String[][]
 *
 * @param env      JNI environment pointer
 * @param array    The target Java String[][] array
 * @param start    Start index in outer array to begin writing (typically 0)
 * @param len      Number of rows to write
 * @param buf      The native buffer (array of String array pointers)
 */
extern void GetString2DArrayRegion(JNIEnv *env, jobjectArray array, jsize start, jsize len, jchar ***buf);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI2D_H__ */
