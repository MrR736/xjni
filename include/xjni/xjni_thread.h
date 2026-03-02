/**
 * @file xjni_thread.h
 * @brief Cross-platform JNI Thread Utility
 *
 * Provides cross-platform thread utilities for C/C++ code interacting with Java via JNI.
 * Includes mutexes, one-time initialization, and realtime timestamp retrieval.
 *
 * @author MrR736
 * @date 2026
 * @copyright GPL-3
 */

#ifndef __XJNI_THREAD_H__
#define __XJNI_THREAD_H__

#ifdef _WIN32
# include <windows.h>

/**
 * @typedef xjni_mutex_t
 * @brief Cross-platform mutex type.
 *
 * On Windows, this is a CRITICAL_SECTION.
 */
typedef CRITICAL_SECTION xjni_mutex_t;

/**
 * @typedef xjni_once_t
 * @brief One-time initialization control for threads.
 *
 * On Windows, this is an INIT_ONCE.
 */
typedef INIT_ONCE xjni_once_t;

/**
 * @typedef xjni_thread_t
 * @brief Handle representing a thread.
 *
 * On Windows, this is a HANDLE returned by CreateThread.
 */
typedef HANDLE xjni_thread_t;

/**
 * @typedef xjni_thread_result_t
 * @brief Return type of a thread function.
 *
 * On Windows, this is a DWORD returned by the thread procedure.
 */
typedef DWORD xjni_thread_result_t;

/** @brief Static initializer for xjni_once_t */
# define XJNI_ONCE_INIT INIT_ONCE_STATIC_INIT

/** @brief Static initializer for xjni_mutex_t */
# define XJNI_MUTEX_INIT {0}

/** @brief Thread cancellation enabled flag (Windows only, for API compatibility) */
# define XJNI_CANCEL_ENABLE  1

/** @brief Thread cancellation disabled flag (Windows only, for API compatibility) */
# define XJNI_CANCEL_DISABLE 0

# ifndef _TIMESPEC_DEFINED
#  define _TIMESPEC_DEFINED
/**
 * @struct timespec
 * @brief Time structure representing seconds and nanoseconds.
 *
 * Needed on Windows because <time.h> does not define timespec by default.
 */
struct timespec {
	time_t	tv_sec;  /**< seconds */
	long	tv_nsec; /**< nanoseconds */
};
# endif

#else /* POSIX */

# include <pthread.h>

/**
 * @typedef xjni_mutex_t
 * @brief Cross-platform mutex type.
 *
 * On POSIX systems, this is a pthread_mutex_t.
 */
typedef pthread_mutex_t xjni_mutex_t;

/**
 * @typedef xjni_once_t
 * @brief One-time initialization control for threads.
 *
 * On POSIX systems, this is a pthread_once_t.
 */
typedef pthread_once_t xjni_once_t;

/**
 * @typedef xjni_thread_t
 * @brief Handle representing a thread.
 *
 * On POSIX systems, this is a pthread_t.
 */
typedef pthread_t xjni_thread_t;

/**
 * @typedef xjni_thread_result_t
 * @brief Return type of a thread function.
 *
 * On POSIX systems, this is void* (or int if you only return status codes).
 */
typedef int xjni_thread_result_t;

/** @brief Static initializer for xjni_once_t */
# define XJNI_ONCE_INIT PTHREAD_ONCE_INIT

/** @brief Static initializer for xjni_mutex_t */
# define XJNI_MUTEX_INIT PTHREAD_MUTEX_INITIALIZER

# ifdef __ANDROID__
/** @brief Thread cancellation enabled flag (Android) */
#  define XJNI_CANCEL_ENABLE  1
/** @brief Thread cancellation disabled flag (Android) */
#  define XJNI_CANCEL_DISABLE 0
# else
/** @brief Thread cancellation enabled flag (POSIX) */
#  define XJNI_CANCEL_ENABLE  PTHREAD_CANCEL_ENABLE
/** @brief Thread cancellation disabled flag (POSIX) */
#  define XJNI_CANCEL_DISABLE PTHREAD_CANCEL_DISABLE
# endif

#endif /* _WIN32 */

#include <stdbool.h>
#include <time.h>

#ifdef _WIN32
#ifdef XJNI_THREAD_BUILD
#define XJNI_THREAD_API __declspec(dllexport)
#else
#define XJNI_THREAD_API __declspec(dllimport)
#endif
#define XJNI_THREAD_CALL __stdcall
#else
#define XJNI_THREAD_API
#define XJNI_THREAD_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Execute a function only once across all threads.
 *
 * @param once_control Pointer to the one-time initialization control variable.
 * @param init_routine Function to call exactly once.
 * @return 0 on success, non-zero on failure.
 */
XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_once(xjni_once_t *once_control, void (*init_routine)(void));

/**
 * @brief Execute a function only once across all threads, with parameters and a return status.
 *
 * This function ensures that the specified initialization routine is called exactly once,
 * even if multiple threads invoke `xjni_oncew` concurrently. Unlike `xjni_once`, this
 * variant allows passing a `Parameter` and a `Context` pointer to the initialization
 * routine and returns a status indicating success or failure.
 *
 * @param once_control Pointer to the one-time initialization control variable (xjni_once_t).
 * @param init_routine Pointer to the initialization function. It should return `true` on success
 *                     and `false` on failure. The function takes `Parameter` and `Context` as arguments.
 * @param Parameter    Pointer to user-defined data to pass as the first argument to `init_routine`.
 * @param Context      Pointer to user-defined data to pass as the second argument to `init_routine`.
 * @return 0 if the initialization function succeeded, -1 if it failed or if invalid arguments
 *         were provided.
 *
 * @note This function is thread-safe and relies on `xjni_once` internally. The `init_routine`
 *       will be executed at most once across all threads that call this function with the
 *       same `once_control`.
 */
XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL
xjni_oncew(xjni_once_t* once_control,
	   bool (*init_routine)(void* Parameter, void* Context),
	   void* Parameter,void* Context);

/**
 * @brief Initialize a mutex.
 * @param mutex Pointer to the mutex to initialize.
 * @return 0 on success, non-zero on failure.
 */
XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_mutex_init(xjni_mutex_t *mutex);

/**
 * @brief Destroy a mutex.
 * @param mutex Pointer to the mutex to destroy.
 * @return 0 on success, non-zero on failure.
 */
XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_mutex_destroy(xjni_mutex_t *mutex);

/**
 * @brief Lock a mutex, blocking if necessary.
 * @param mutex Pointer to the mutex to lock.
 * @return 0 on success, non-zero on failure.
 */
XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_mutex_lock(xjni_mutex_t *mutex);

/**
 * @brief Unlock a mutex.
 * @param mutex Pointer to the mutex to unlock.
 * @return 0 on success, non-zero on failure.
 */
XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_mutex_unlock(xjni_mutex_t *mutex);

/**
 * @brief Try to lock a mutex without blocking.
 * @param mutex Pointer to the mutex to try-lock.
 * @return 0 if lock acquired, non-zero if already locked.
 */
XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_mutex_trylock(xjni_mutex_t *mutex);

/**
 * @brief Lock a mutex with a timeout.
 * @param mutex Pointer to the mutex to lock.
 * @param abstime Absolute time to wait until giving up.
 * @return 0 if lock acquired, non-zero on timeout or failure.
 */
XJNI_THREAD_API int XJNI_THREAD_CALL xjni_mutex_timedlock(xjni_mutex_t *mutex, const struct timespec *abstime);

/**
 * @brief Get the current realtime clock.
 * @param ts Pointer to a timespec struct to populate with current time.
 */
XJNI_THREAD_API void XJNI_THREAD_CALL xjni_get_realtime(struct timespec *ts);

/**
 * @brief Create a new thread.
 *
 * @param thread Pointer to store the newly created thread handle.
 * @param start_routine Function pointer that represents the thread entry function.
 * @param arg Argument passed to the thread function.
 * @return xjni_thread_result_t
 *         - Windows: DWORD, 0 on success, GetLastError() code on failure.
 *         - POSIX: 0 on success, error number on failure.
 */
XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_create(xjni_thread_t *thread,void *(*start_routine)(void*),void *arg);

/**
 * @brief Wait for a thread to finish execution and optionally retrieve its result.
 *
 * @param thread Thread handle to join.
 * @param retval Pointer to store the thread's return value (can be NULL).
 * @return xjni_thread_result_t
 *         - Windows: DWORD, 0 on success, GetLastError() code on failure.
 *         - POSIX: 0 on success, error number on failure.
 */
XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_join(xjni_thread_t thread, void **retval);

/**
 * @brief Get the handle of the current thread.
 *
 * @return xjni_thread_t
 *         - Windows: HANDLE of the calling thread (note: pseudo-handle may be returned).
 *         - POSIX: pthread_t of the calling thread.
 */
XJNI_THREAD_API xjni_thread_t XJNI_THREAD_CALL xjni_self(void);

/**
 * @brief Detach a thread, allowing its resources to be reclaimed automatically.
 *
 * @param thread Thread handle to detach.
 * @return xjni_thread_result_t
 *         - Windows: DWORD, 0 on success, GetLastError() on failure.
 *         - POSIX: 0 on success, error number on failure.
 */
XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_detach(xjni_thread_t thread);

/**
 * @brief Check if two thread handles represent the same thread.
 *
 * @param t1 First thread handle.
 * @param t2 Second thread handle.
 * @return int Non-zero if threads are equal, 0 otherwise.
 */
XJNI_THREAD_API int XJNI_THREAD_CALL xjni_equal(xjni_thread_t t1, xjni_thread_t t2);

/**
 * @brief Request cancellation of a thread.
 *
 * @param th Thread handle to cancel.
 * @return xjni_thread_result_t
 *         - Windows: ENOSYS (cancellation unsupported).
 *         - POSIX/Android: 0 on success, error number on failure.
 */
XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_cancel(xjni_thread_t th);

/**
 * @brief Set the thread's cancellation type.
 *
 * @param type Cancellation type (XJNI_CANCEL_ENABLE or XJNI_CANCEL_DISABLE).
 * @param oldtype Optional pointer to store previous cancellation type.
 * @return int 0 on success, or platform-specific error code.
 */
XJNI_THREAD_API int XJNI_THREAD_CALL xjni_setcanceltype(int type, int *oldtype);

/**
 * @brief Set the thread's cancellation state.
 *
 * @param state Cancellation state (XJNI_CANCEL_ENABLE or XJNI_CANCEL_DISABLE).
 * @param oldstate Optional pointer to store previous cancellation state.
 * @return int 0 on success, or platform-specific error code.
 */
XJNI_THREAD_API int XJNI_THREAD_CALL xjni_setcancelstate(int state, int *oldstate);

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_THREAD_H__ */
