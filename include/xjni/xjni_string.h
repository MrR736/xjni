/**
 * @file xjni_string.h
 * @brief Extern JNI Utility - comprehensive utilities for JNI string, memory, and encoding handling.
 *
 * Provides:
 * - UTF-8 ↔ UTF-16 (jchar) conversions
 * - UTF-8 ↔ wchar_t conversions
 * - jchar-based string utilities
 * - Memory helpers compatible with JNI types
 *
 * Encoding assumptions:
 * - jchar is 16-bit UTF-16
 * - wchar_t is UTF-16 on Windows
 * - wchar_t is UTF-32 on POSIX systems
 *
 * All returned dynamically allocated buffers must be freed using free().
 *
 * @author MrR736
 * @date 2026
 * @copyright GPL-3
 */

#ifndef __XJNI_STRING_H__
#define __XJNI_STRING_H__

#include <wchar.h>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup XJNI_Memory Memory Utilities
 *  @brief Standard memory operations for JNI
 *  @{
 */
JNIEXPORT void* JNICALL jmemcpy(void *dest,const void *src,size_t n);
JNIEXPORT void* JNICALL jmemmove(void *dest,const void *src,size_t n);
JNIEXPORT void* JNICALL jmemchr(const void *s,jint c,size_t n);
JNIEXPORT void* JNICALL jmemset(void *s,jint c,size_t n);
JNIEXPORT jint JNICALL jmemcmp(const void *cs,const void *ct,size_t count);
/** @} */

/** @defgroup XJNI_String jchar String Utilities
 *  @brief Standard string operations for jchar arrays
 *  @{
 */

/**
 * @brief Convert a single jchar to UTF-8 char
 * @param c Input jchar
 * @param out Output buffer for UTF-8 char(s)
 * @param out_size Size of output buffer
 * @return Number of bytes written
 */
JNIEXPORT jint JNICALL xjni_tochar(const jchar c, char* out, size_t out_size);

/**
 * @brief Convert UTF-8 char array to jchar
 * @param utf8 Input UTF-8 char array
 * @param utf8_len Length of input array
 * @param out Output jchar buffer
 * @return Number of jchars written
 */
JNIEXPORT jint JNICALL xjni_tojchar(const char* utf8, size_t utf8_len, jchar* out);

/**
 * @brief Convert UTF-16 (jchar*) string to UTF-8 string.
 *
 * Converts a null-terminated UTF-16 encoded string (JNI jchar*)
 * into a newly allocated null-terminated UTF-8 string.
 *
 * Surrogate pairs are combined into proper 4-byte UTF-8 sequences.
 *
 * @param c Null-terminated UTF-16 input string.
 * @return Newly allocated UTF-8 string that must be freed with free(),
 *         or NULL if the input is NULL, contains invalid surrogate pairs,
 *         or memory allocation fails.
 *
 * @note The returned string is always null-terminated.
 */
JNIEXPORT char* JNICALL xjni_tostring(const jchar* c);

/**
 * @brief Convert UTF-8 string to UTF-16 (jchar*) string.
 *
 * Converts a null-terminated UTF-8 encoded string into a newly
 * allocated null-terminated UTF-16 string (JNI jchar*).
 *
 * 4-byte UTF-8 sequences are converted into UTF-16 surrogate pairs.
 *
 * @param c Null-terminated UTF-8 input string.
 * @return Newly allocated UTF-16 string (jchar*) that must be freed with free(),
 *         or NULL if the input is NULL, contains invalid UTF-8 sequences,
 *         or memory allocation fails.
 *
 * @note The returned string is always null-terminated.
 */
JNIEXPORT jchar* JNICALL xjni_tojstring(const char* c);

/**
 * @brief Copy UTF-8 string to jchar array
 * @param src Input UTF-8 string
 * @param dst Output jchar buffer
 * @param dstlen [in/out] Size of dst buffer, updated with written length
 * @return JNI_TRUE on success, JNI_FALSE on failure
 */
JNIEXPORT jboolean JNICALL xjni_fromstring(const char *src, jchar *dst, size_t *dstlen);

/**
 * @brief Copy jchar array to UTF-8 string
 * @param src Input jchar array
 * @param dst Output UTF-8 buffer
 * @param dstlen [in/out] Size of dst buffer, updated with written length
 * @return JNI_TRUE on success, JNI_FALSE on failure
 */
JNIEXPORT jboolean JNICALL xjni_fromjstring(const jchar *src, char *dst, size_t *dstlen);

JNIEXPORT size_t JNICALL jstrlen(const jchar* __s);
JNIEXPORT size_t JNICALL jstrnlen(const jchar * s,size_t count);
JNIEXPORT jchar* JNICALL jstrchr(const jchar *s,jint c);
JNIEXPORT jchar* JNICALL jstrchrnul(const jchar *__s,jint __c);
JNIEXPORT jchar* JNICALL jstrrchr(const jchar *s,jint c);
JNIEXPORT jint JNICALL jstrcmp(const jchar *cs,const jchar *ct);
JNIEXPORT jchar* JNICALL jstrcpy(jchar* __dest,const jchar* __src);
JNIEXPORT size_t JNICALL jstrlcpy(jchar *dest,const jchar *src,size_t size);
JNIEXPORT jchar* JNICALL jstrcat(jchar* __dest,const jchar* __src);
JNIEXPORT size_t JNICALL jstrlcat(jchar *dest,const jchar *src,size_t size);
JNIEXPORT jint JNICALL jstrncmp(const jchar *cs,const jchar *ct,size_t count);
JNIEXPORT jchar* JNICALL jstrpbrk(const jchar * cs,const jchar * ct);
JNIEXPORT jchar* JNICALL jstrstr(const jchar *s1,const jchar *s2);
JNIEXPORT size_t JNICALL jstrcspn(const jchar *__s,const jchar *__reject);
JNIEXPORT size_t JNICALL jstrspn(const jchar *s,const jchar *accept);
JNIEXPORT jchar* JNICALL jstrdup(const jchar *s);
JNIEXPORT jchar* JNICALL jstrndup(const jchar *__string,size_t __n);
JNIEXPORT jint JNICALL jstrcoll(const jchar *__s1,const jchar *__s2);
JNIEXPORT size_t JNICALL jstrxfrm(jchar* __dest,const jchar* __src,size_t __n);
JNIEXPORT jchar* JNICALL jstrtok(jchar* __s,const jchar* __delim);
JNIEXPORT void JNICALL jstrreverse(jchar* __str);
JNIEXPORT jchar* JNICALL jstrrev(const jchar* jstr);
/** @} */

/** @defgroup XJNI_ConvertString Convert & Encoding Utilities
 *  @brief UTF-8, UTF-16 (jchar), and wide-character conversion utilities.
 *
 *  Provides platform-aware conversion between:
 *  - UTF-8 (char*)
 *  - UTF-16 (jchar*)
 *  - Wide-character strings (wchar_t*)
 *
 *  @{
 */

/**
 * @brief Convert wide-character string (wchar_t*) to UTF-8 string.
 *
 * Converts a null-terminated wide-character string into a newly allocated
 * null-terminated UTF-8 encoded string.
 *
 * Platform behavior:
 * - On Windows: wchar_t is UTF-16; conversion uses WideCharToMultiByte().
 * - On POSIX systems: wchar_t is UTF-32; conversion uses wcstombs().
 *
 * @param wcs Null-terminated wide-character input string.
 * @return Newly allocated UTF-8 string that must be freed with free(),
 *         or NULL if conversion fails or memory allocation fails.
 *
 * @note The returned string is always null-terminated.
 */
JNIEXPORT char* JNICALL xjni_wcstombs(const wchar_t *wcs);


/**
 * @brief Convert UTF-8 string to wide-character string (wchar_t*).
 *
 * Converts a null-terminated UTF-8 encoded string into a newly allocated
 * null-terminated wide-character string.
 *
 * Platform behavior:
 * - On Windows: wchar_t is UTF-16; conversion uses MultiByteToWideChar().
 * - On POSIX systems: wchar_t is UTF-32; conversion uses mbstowcs().
 *
 * @param mbs Null-terminated UTF-8 input string.
 * @return Newly allocated wide-character string that must be freed with free(),
 *         or NULL if the input contains invalid UTF-8 sequences or memory allocation fails.
 *
 * @note The returned string is always null-terminated.
 */
JNIEXPORT wchar_t* JNICALL xjni_mbstowcs(const char *mbs);


/**
 * @brief Convert UTF-16 (jchar*) string to wide-character string (wchar_t*).
 *
 * Converts a null-terminated UTF-16 encoded string (JNI jchar*)
 * into a newly allocated null-terminated wide-character string.
 *
 * Platform behavior:
 * - On Windows: jchar and wchar_t are both UTF-16; conversion is a direct copy.
 * - On POSIX systems: jchar is UTF-16 and wchar_t is UTF-32; surrogate pairs
 *   are decoded and combined into single UTF-32 code points.
 *
 * @param c Null-terminated UTF-16 input string (jchar*).
 * @return Newly allocated wide-character string that must be freed with free(),
 *         or NULL if invalid surrogate pairs are detected or memory allocation fails.
 *
 * @note The returned string is always null-terminated.
 */
JNIEXPORT wchar_t* JNICALL xjni_jmbstowcs(const jchar* c);


/**
 * @brief Convert wide-character string (wchar_t*) to UTF-16 (jchar*) string.
 *
 * Converts a null-terminated wide-character string into a newly allocated
 * null-terminated UTF-16 encoded string suitable for JNI usage.
 *
 * Platform behavior:
 * - On Windows: wchar_t and jchar are both UTF-16; conversion is a direct copy.
 * - On POSIX systems: wchar_t is UTF-32; code points above U+FFFF are encoded
 *   as UTF-16 surrogate pairs.
 *
 * @param c Null-terminated wide-character input string.
 * @return Newly allocated UTF-16 string (jchar*) that must be freed with free(),
 *         or NULL if invalid Unicode code points are encountered or memory allocation fails.
 *
 * @note The returned string is always null-terminated.
 */
JNIEXPORT jchar* JNICALL xjni_jwcstombs(const wchar_t* c);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_STRING_H__ */
