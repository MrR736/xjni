/**
 * @file xjnidef.h
 * @brief Extern Define Utility Header
 *
 * Provides portable CPU detection, compiler builtin wrappers, and utility macros for C/C++ projects.
 * Includes modern/legacy CPU optimizations, bit manipulation helpers, branch prediction hints, and
 * compile-time type utilities.
 *
 * @author MrR736
 * @date 2026
 * @copyright MIT
 */

#ifndef __XJNIDEF_H__
#define __XJNIDEF_H__

#include <stdint.h>
#include <stddef.h>

/* ----------------- C Standard Versions ----------------- */
#define STDC94		199409L  /**< C90 amendment (ISO/IEC 9899:1990 + 1994) */
#define STDC99		199901L  /**< C99 (ISO/IEC 9899:1999) */
#define STDC11		201112L  /**< C11 (ISO/IEC 9899:2011) */
#define STDC17		201710L  /**< C17 / C18 (ISO/IEC 9899:2017/2018) */
#define STDC18		STDC17   /**< Alias for C17 */
#define STDC23		202311L  /**< C23 (ISO/IEC 9899:2023) */

/* ----------------- C++ Standard Versions ----------------- */
#define STDC98PP	199711L  /**< C++98 */
#define STDC03PP	STDC98PP /**< C++03 */
#define STDC11PP	201103L  /**< C++11 */
#define STDC14PP	201402L  /**< C++14 */
#define STDC17PP	201703L  /**< C++17 */
#define STDC20PP	202002L  /**< C++20 */
#define STDC23PP	202300L  /**< C++23 */
#define STDC26PP	202600L  /**< C++26 */

/* ----------------- C Standard Version Comparison Macros ----------------- */
#ifdef __STDC_VERSION__
# define STDC_VERSION_GE(v) (__STDC_VERSION__ >= (v))
# define STDC_VERSION_LE(v) (__STDC_VERSION__ <= (v))
# define STDC_VERSION_EQ(v) (__STDC_VERSION__ == (v))
# define STDC_VERSION_NE(v) (__STDC_VERSION__ != (v))
# define STDC_VERSION_LT(v) (__STDC_VERSION__ < (v))
# define STDC_VERSION_GT(v) (__STDC_VERSION__ > (v))
#else
# define STDC_VERSION_GE(v) 0
# define STDC_VERSION_LE(v) 0
# define STDC_VERSION_EQ(v) 0
# define STDC_VERSION_NE(v) 1
# define STDC_VERSION_LT(v) 0
# define STDC_VERSION_GT(v) 0
#endif

/* ----------------- C++ Standard Version Comparison Macros ----------------- */
#ifdef __cplusplus
# define CPLUSPLUS_GE(v) (__cplusplus >= (v))
# define CPLUSPLUS_LE(v) (__cplusplus <= (v))
# define CPLUSPLUS_EQ(v) (__cplusplus == (v))
# define CPLUSPLUS_NE(v) (__cplusplus != (v))
# define CPLUSPLUS_LT(v) (__cplusplus < (v))
# define CPLUSPLUS_GT(v) (__cplusplus > (v))
#else
# define CPLUSPLUS_GE(v) 0
# define CPLUSPLUS_LE(v) 0
# define CPLUSPLUS_EQ(v) 0
# define CPLUSPLUS_NE(v) 1
# define CPLUSPLUS_LT(v) 0
# define CPLUSPLUS_GT(v) 0
#endif

/**
 * @defgroup XJNI_Architecture Architecture Detection
 * @brief Macros to detect architecture
 * @{
 */

/** @brief Use 64-bit pointer architecture */
#if defined(UINTPTR_MAX)
# if UINTPTR_MAX == UINT64_MAX
#  define __XJNI_x64__ 1
# elif UINTPTR_MAX == UINT32_MAX
#  define __XJNI_x32__ 1
# else
#  error "Unsupported pointer size."
# endif
#else
# if defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64) || defined(__aarch64__) || defined(__LP64__) || \
     defined(__amd64__) || (defined(__WORDSIZE) && __WORDSIZE == 64)
#  define __XJNI_x64__ 1
# else
#  define __XJNI_x32__ 1
# endif
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64) || defined(__amd64__)
# define __XJNI_x86_64__ 1
#endif

#if defined(_M_I86) || defined(__i386__)
# define __XJNI_i386__ 1
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
# define __XJNI_ARM64__ 1
#elif defined(__arm__) || defined(_M_ARM)
# define __XJNI_ARM__ 1
#endif

#if defined(__mips64)
# define __XJNI_MIPS64__ 1
#elif defined(__mips__) || defined(__mips)
# define __XJNI_MIPS__ 1
#endif

/** @} */

#if defined(__has_feature)
# if __has_feature(address_sanitizer)
#  define __XJNI_ASAN__
# endif
# if __has_feature(thread_sanitizer)
#  define __XJNI_TSAN__
# endif
#elif defined(__SANITIZE_ADDRESS__)
# define __XJNI_ASAN__
#elif defined(__SANITIZE_THREAD__)
# define __XJNI_TSAN__
#endif

/**
 * @defgroup XJNI_CPU modern/legacy CPU Detection
 * @brief Macros to modern/legacy CPU
 * @{
 */

#if defined(__XJNI_x64__) && defined(__AVX2__)
# define __XJNI_SIMD_AVX2__ 1
#elif defined(__XJNI_x64__) && defined(__AVX__)
# define __XJNI_SIMD_AVX__ 1
#elif defined(__XJNI_x64__) && defined(__SSE4_2__)
# define __XJNI_SIMD_SSE42__ 1
#else
# define __XJNI_SIMD_NONE__ 1
#endif

#if !defined(__XJNI_SIMD_NONE__)
# define __XJNI_MODERN_CPU__ 1
#else
# define __XJNI_LEGACY_CPU__ 1
#endif

/** @} */

/**
 * @defgroup XJNI_Builtins Compiler Builtin Wrappers
 * @brief Portable wrappers for common compiler builtins
 * @{
 */

/** @brief Check if a compiler builtin exists */
#ifndef __xjni_builtin
# ifdef __has_builtin
#  define __xjni_builtin(x) __has_builtin(x)
# else
#  define __xjni_builtin(x) 0
# endif
#endif

/**
 * @brief Type compatibility check
 *
 * Checks if two types are compatible.
 *
 * @param t1 First type
 * @param t2 Second type
 * @return 1 if types are compatible, 0 otherwise
 */
#ifndef __xjni_types_compatible_p
# ifdef __cplusplus
extern "C++" {
#  if CPLUSPLUS_EQ(STDC20PP)
#   include <type_traits>
#   define __xjni_types_compatible_p(t1, t2) \
	std::is_same_v<std::remove_cvref_t<t1>, std::remove_cvref_t<t2>>
#  elif CPLUSPLUS_GE(STDC17PP)
#   include <type_traits>
#   define __xjni_types_compatible_p(t1, t2) \
	std::is_same_v<std::remove_cv_t<t1>, std::remove_cv_t<t2>>
#  elif CPLUSPLUS_GE(STDC11PP)
#   include <type_traits>
#   define __xjni_types_compatible_p(t1, t2) \
	std::is_same<typename std::remove_cv<t1>::type,typename std::remove_cv<t2>::type>::value
#  else
template <typename T1, typename T2>
struct __xjni_types_compatible_impl {
    static const bool value = false;
};
template <typename T>
struct __xjni_types_compatible_impl<T, T> {
    static const bool value = true;
};
template <typename T>
struct __xjni_types_compatible_impl<const T, T> {
    static const bool value = true;
};
template <typename T>
struct __xjni_types_compatible_impl<T, const T> {
    static const bool value = true;
};
template <typename T>
struct __xjni_types_compatible_impl<volatile T, T> {
    static const bool value = true;
};
template <typename T>
struct __xjni_types_compatible_impl<T, volatile T> {
    static const bool value = true;
};
template <typename T>
struct __xjni_types_compatible_impl<const volatile T, T> {
    static const bool value = true;
};
template <typename T>
struct __xjni_types_compatible_impl<T, const volatile T> {
    static const bool value = true;
};
#   define __xjni_types_compatible_p(t1, t2) __xjni_types_compatible_impl<t1, t2>::value
#  endif
} // extern "C++"
# else /* C */
#  if __xjni_builtin(__builtin_types_compatible_p)
#   define __xjni_types_compatible_p(t1, t2) __builtin_types_compatible_p(t1, t2)
#  else
#   define __xjni_types_compatible_p(t1, t2) 0
#  endif
# endif
#endif

/**
 * @brief Compile-time conditional expression
 * @param cond Condition to evaluate
 * @param A Expression if cond is true
 * @param B Expression if cond is false
 */
#ifndef __xjni_choose_expr
# if __xjni_builtin(__builtin_choose_expr)
#  define __xjni_choose_expr(cond, A, B) __builtin_choose_expr(cond, A, B)
# else
#  define __xjni_choose_expr(cond, A, B) ((cond) ? (A) : (B))
# endif
#endif

/**
 * @brief Branch prediction hint
 * @param expr Expression to evaluate
 * @param expected Expected value (0 or 1)
 * @return The value of expr
 */
#ifndef __xjni_expect
# if __xjni_builtin(__builtin_expect)
#  define __xjni_expect(expr, expected) __builtin_expect(expr, expected)
# else
#  define __xjni_expect(expr, expected) (expr)
# endif
#endif

/**
 * @brief Count set bits in unsigned int
 * @param x Input value
 * @return Number of bits set to 1
 */
#ifndef __xjni_popcount
# if __xjni_builtin(__builtin_popcount)
#  define __xjni_popcount(x) __builtin_popcount(x)
# else
#  ifdef __XJNI_MODERN_CPU__
static inline int __xjni_popcount(unsigned int x) {
	int count; for(count=0;x;x&=x-1) count++; return count;
}
#  else
static inline int __xjni_popcount(unsigned int x) {
	int count=0; while(x){ x&=x-1; count++; } return count;
}
#  endif
# endif
#endif

/**
 * @brief Count set bits in unsigned long
 */
#ifndef __xjni_popcountl
# if __xjni_builtin(__builtin_popcountl)
#  define __xjni_popcountl(x) __builtin_popcountl(x)
# else
#  ifdef __XJNI_MODERN_CPU__
static inline int __xjni_popcountl(unsigned long x) {
	int count; for(count=0;x;x&=x-1) count++; return count;
}
#  else
static inline int __xjni_popcountl(unsigned long x) {
	int count=0; while(x){ x&=x-1; count++; } return count;
}
#  endif
# endif
#endif

/**
 * @brief Count set bits in unsigned long long
 */
#ifndef __xjni_popcountll
# if __xjni_builtin(__builtin_popcountll)
#  define __xjni_popcountll(x) __builtin_popcountll(x)
# else
#  ifdef __XJNI_MODERN_CPU__
static inline int __xjni_popcountll(unsigned long long x) {
	int count; for(count=0;x;x&=x-1) count++; return count;
}
#  else
static inline int __xjni_popcountll(unsigned long long x) {
	int count=0; while(x){ x&=x-1; count++; } return count;
}
#  endif
# endif
#endif

/**
 * @brief Count leading zeros in unsigned int
 */
#ifndef __xjni_clz
# if __xjni_builtin(__builtin_clz)
#  define __xjni_clz(x) __builtin_clz(x)
# else
static inline int __xjni_clz(unsigned int x) {
	if(x==0) return 32; int n=0; unsigned int mask=1U<<31;
	while((x&mask)==0){ n++; mask>>=1; } return n;
}
# endif
#endif

/**
 * @brief Count leading zeros in unsigned long
 */
#ifndef __xjni_clzl
# if __xjni_builtin(__builtin_clzl)
#  define __xjni_clzl(x) __builtin_clzl(x)
# else
static inline int __xjni_clzl(unsigned long x) {
	if(x==0) return sizeof(unsigned long)*8; int n=0; unsigned long mask=1UL<<(sizeof(unsigned long)*8-1);
	while((x&mask)==0){ n++; mask>>=1; } return n;
}
# endif
#endif

/**
 * @brief Count leading zeros in unsigned long long
 */
#ifndef __xjni_clzll
# if __xjni_builtin(__builtin_clzll)
#  define __xjni_clzll(x) __builtin_clzll(x)
# else
static inline int __xjni_clzll(unsigned long long x) {
	if(x==0) return 64; int n=0; unsigned long long mask=1ULL<<63;
	while((x&mask)==0){ n++; mask>>=1; } return n;
}
# endif
#endif

/**
 * @brief Marks code as unreachable
 *
 * Hints the compiler that this code path is not reachable.
 * @note Undefined behavior if reached at runtime.
 */
#ifndef __xjni_unreachable
# if __xjni_builtin(__builtin_unreachable)
#  define __xjni_unreachable() __builtin_unreachable()
# else
#  include <stdlib.h>
#  define __xjni_unreachable() abort()
# endif
#endif

/** @} */


/**
 * @defgroup XJNI_MACROS_UTILITY Macros Utility
 * @brief Macros Utility
 * @{
 */

#ifndef arrayof
# define arrayof(x) (sizeof(x) / sizeof(x[0]))
#endif

#ifdef __cplusplus
# define xusing(n,t) using n = t
#else
# define xusing(n,t) typedef t n
#endif

#define xtypedef(t,n) xusing(n,t)

#ifdef __cplusplus
# define XJNI_BILUDER_ENUM(n,t) n : t
#else
# if STDC_VERSION_GE(STDC23)
#  define XJNI_BILUDER_ENUM(n,t) n : t
# else
#  define XJNI_BILUDER_ENUM(n,t) n
# endif
#endif

#define XJNI_ENUM(n) XJNI_BILUDER_ENUM(n,int)
/** @} */

/**
 * @defgroup JNI_VERSION_UTILITY JNI Version Utility
 * @brief JNI Version Utility
 * @{
 */
#define JNI_VERSION_AT_LEAST(ver, min) ((ver) >= (min))
#define JNI_VERSION_AT_MOST(ver, max) ((ver) <= (max))
#define JNI_VERSION_EQ(ver1, ver2) ((ver1) == (ver2))
#define JNI_VERSION_NE(ver1, ver2) ((ver1) != (ver2))
/** @} */

#if defined(__GNUC__)
# define XJNI_EXTENSION __extension__
#else
# define XJNI_EXTENSION
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
# define XJNI_EXPORT_API __declspec(dllimport)
# define XJNI_IMPORT_API __declspec(dllimport)
# if defined(XJNI_BUILD)
#  define XJNI_API XJNI_EXPORT_API
# else
#  define XJNI_API XJNI_IMPORT_API
# endif
# define XJNI_PRIVATE_API
#elif defined(__GNUC__) || defined(__clang__)
# if defined(__has_attribute)
#  if __has_attribute(visibility)
#   define XJNI_EXPORT_API __attribute__((visibility("default")))
#   define XJNI_IMPORT_API __attribute__((visibility("default")))
#   define XJNI_API __attribute__((visibility("default")))
#   define XJNI_PRIVATE_API __attribute__((visibility("hidden")))
#  else
#   define XJNI_EXPORT_API
#   define XJNI_IMPORT_API
#   define XJNI_API
#   define XJNI_PRIVATE_API
#  endif
# else
#  define XJNI_EXPORT_API
#  define XJNI_IMPORT_API
#  define XJNI_API
#  define XJNI_PRIVATE_API
# endif
#else
# define XJNI_EXPORT_API
# define XJNI_IMPORT_API
# define XJNI_API
# define XJNI_PRIVATE_API
#endif

#endif /* __XJNIDEF_H__ */
