#include <stdint.h>

#define XJNI_THREAD_BUILD 1
#include <xjni_thread.h>

#ifdef _WIN32
typedef struct { void (*fn)(void); } once_ctx;
static BOOL CALLBACK once_wrapper(PINIT_ONCE InitOnce, PVOID Parameter, PVOID *Context) {
	(void)Context;
	once_ctx* ctx = (once_ctx*)Parameter;
	if (!ctx || !ctx->fn) return FALSE;
	ctx->fn();
	return TRUE;
}
#endif

#ifdef _WIN32
# if defined(_MSC_VER)
#  define XJNI_TLS __declspec(thread)
# else
#  define XJNI_TLS _Thread_local
# endif
#else
# define XJNI_TLS __thread
#endif

typedef struct {
	bool (*fn)(void* Parameter, void* Context);
	void* Parameter;
	void* Context;
	int result; // 0=success, -1=failure
} xjni_once_ctx_t;

typedef struct xjni_once_map_s {
	xjni_once_t* once_control;
	xjni_once_ctx_t* ctx;
	struct xjni_once_map_s* next;
} xjni_once_map_t;

static xjni_mutex_t g_once_map_mutex;
static xjni_once_map_t* g_once_map = NULL;

// Internal wrapper called by pthread_once
static void oncew_internal_wrapper(void) {
	xjni_once_ctx_t* ctx = NULL;

	xjni_mutex_lock(&g_once_map_mutex);
	if (g_once_map) {
		ctx = g_once_map->ctx; // head of list
	}
	xjni_mutex_unlock(&g_once_map_mutex);

	if (ctx && ctx->fn) {
		ctx->result = ctx->fn(ctx->Parameter, ctx->Context) ? 0 : -1;
	}
}

XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL
xjni_oncew(xjni_once_t* once_control,bool (*init_routine)(void* Parameter, void* Context),void* Parameter, void* Context) {
	if (!once_control || !init_routine) return -1;

	xjni_mutex_init(&g_once_map_mutex);

	xjni_once_ctx_t ctx = {init_routine, Parameter, Context, -1};
	xjni_once_map_t map = {once_control, &ctx, NULL};

	xjni_mutex_lock(&g_once_map_mutex);
	map.next = g_once_map;
	g_once_map = &map;
	xjni_mutex_unlock(&g_once_map_mutex);

	// Call xjni_once which uses the wrapper with no arguments
	xjni_once(once_control, oncew_internal_wrapper);

	xjni_mutex_lock(&g_once_map_mutex);
	xjni_once_map_t** iter = &g_once_map;
	while (*iter) {
		if (*iter == &map) { *iter = map.next; break; }
		iter = &(*iter)->next;
	}
	xjni_mutex_unlock(&g_once_map_mutex);

	xjni_mutex_destroy(&g_once_map_mutex);

	return ctx.result;
}

XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_once(xjni_once_t *once_control, void (*init_routine)(void)) {
#ifdef _WIN32
	once_ctx ctx = { init_routine };
	BOOL result = InitOnceExecuteOnce(once_control, once_wrapper, &ctx, NULL);
	return result ? 0 : -1;
#else
	return pthread_once(once_control, init_routine);
#endif
}

XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_mutex_init(xjni_mutex_t *mutex) {
#ifdef _WIN32
#if _WIN32_WINNT >= 0x0403
	if (!InitializeCriticalSectionAndSpinCount(mutex, 4000)) return ENOMEM;
#else
	InitializeCriticalSection(mutex);
#endif
	return 0;
#else
	return pthread_mutex_init(mutex, NULL);
#endif
}

XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_mutex_destroy(xjni_mutex_t *mutex) {
#ifdef _WIN32
	DeleteCriticalSection(mutex);
	return 0;
#else
	return pthread_mutex_destroy(mutex);
#endif
}

XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_mutex_lock(xjni_mutex_t *mutex) {
#ifdef _WIN32
	EnterCriticalSection(mutex);
	return 0;
#else
	return pthread_mutex_lock(mutex);
#endif
}

XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_mutex_unlock(xjni_mutex_t *mutex) {
#ifdef _WIN32
	LeaveCriticalSection(mutex);
	return 0;
#else
	return pthread_mutex_unlock(mutex);
#endif
}

XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_mutex_trylock(xjni_mutex_t *mutex) {
#ifdef _WIN32
	return TryEnterCriticalSection(mutex) ? 0 : EBUSY;
#else
	return pthread_mutex_trylock(mutex);
#endif
}

XJNI_THREAD_API void XJNI_THREAD_CALL xjni_get_realtime(struct timespec *ts) {
#ifdef _WIN32
	FILETIME ft;
	ULARGE_INTEGER uli;
	GetSystemTimeAsFileTime(&ft);
	uli.LowPart  = ft.dwLowDateTime;
	uli.HighPart = ft.dwHighDateTime;
	uint64_t t = uli.QuadPart - 116444736000000000ULL;
	ts->tv_sec  = t / 10000000ULL;
	ts->tv_nsec = (t % 10000000ULL) * 100;
#else
	clock_gettime(CLOCK_REALTIME, ts);
#endif
}

XJNI_THREAD_API int XJNI_THREAD_CALL xjni_mutex_timedlock(xjni_mutex_t *mutex, const struct timespec *abstime) {
#ifdef _WIN32
	DWORD timeout_ms;
	if (abstime) {
		struct timespec now;
		xjni_get_realtime(&now);
		long sec_diff = abstime->tv_sec - now.tv_sec;
		long nsec_diff = abstime->tv_nsec - now.tv_nsec;
		if (nsec_diff < 0) {
			nsec_diff += 1000000000;
			sec_diff -= 1;
		}
		int64_t total_ms64 = (int64_t)sec_diff * 1000 + nsec_diff / 1000000;
		if (total_ms64 <= 0) return ETIMEDOUT;
		timeout_ms = (DWORD)total_ms64;
	} else timeout_ms = INFINITE;
	DWORD elapsed = 0;
	const DWORD interval = 1;
	while (elapsed < timeout_ms) {
		if (TryEnterCriticalSection(mutex)) return 0;
		Sleep(interval);
		elapsed += interval;
	}
	if (TryEnterCriticalSection(mutex)) return 0;
	return ETIMEDOUT;
#else
	return pthread_mutex_timedlock(mutex, abstime);
#endif
}

#ifdef __ANDROID__
#include <signal.h>
#define SETUP_SIG_HANDLER(sig) \
	do { \
		struct sigaction sa; \
		sa.sa_handler = signal_handler; \
		sa.sa_flags = 0; \
		sigemptyset(&sa.sa_mask); \
		sigaction(sig, &sa, NULL); \
	} while (0)

#define ENABLE_SIGNAL(sig) \
	do { \
		SETUP_SIG_HANDLER(sig);  /* Ensure SIG is handled by empty_signal_handler */ \
		sigset_t set; \
		sigemptyset(&set); \
		sigaddset(&set, sig); \
		pthread_sigmask(SIG_UNBLOCK, &set, NULL); \
	} while (0)

#define DISABLE_SIGNAL(sig) \
	do { \
		sigset_t set; \
		sigemptyset(&set); \
		sigaddset(&set, sig); \
		pthread_sigmask(SIG_BLOCK, &set, NULL); \
	} while (0)
#endif

#ifdef _WIN32
typedef struct {
	void *(*start_routine)(void *);
	void *arg;
	void *result;
} xjni_thread_start_t;

static DWORD WINAPI xjni_thread_wrapper(LPVOID param) {
	xjni_thread_start_t *data = (xjni_thread_start_t *)param;
	data->result = data->start_routine(data->arg);
	return 0;
}
#endif

XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_create(xjni_thread_t *thread,void *(*start_routine)(void *),void *arg) {
#ifdef _WIN32
	xjni_thread_start_t *data = malloc(sizeof(*data));
	if (!data) return (xjni_thread_result_t)-1;
	data->start_routine = start_routine;
	data->arg = arg;
	data->result = NULL;
	HANDLE h = CreateThread(NULL, 0, xjni_thread_wrapper, data, 0, NULL);
	if (!h) {
		free(data);
		return GetLastError();
	}
	*thread = h;
	return (xjni_thread_result_t)0;
#else
	return pthread_create(thread, NULL, start_routine, arg);
#endif
}

XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_join(xjni_thread_t thread, void **retval) {
#ifdef _WIN32
	DWORD wait_res = WaitForSingleObject(thread, INFINITE);
	if (wait_res != WAIT_OBJECT_0) return wait_res;
	xjni_thread_start_t *data;
	if (!GetExitCodeThread(thread, (LPDWORD)&data)) return GetLastError();
#else
	return pthread_join(thread, retval);
#endif
}

XJNI_THREAD_API xjni_thread_t XJNI_THREAD_CALL xjni_self(void) {
#ifdef _WIN32
	return OpenThread(THREAD_ALL_ACCESS,FALSE,GetCurrentThreadId());
#else
	return pthread_self();
#endif
}

XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_detach(xjni_thread_t thread) {
#ifdef _WIN32
	if (!CloseHandle(thread)) return GetLastError();
	return 0;
#else
	return pthread_detach(thread);
#endif
}

XJNI_THREAD_API int XJNI_THREAD_CALL xjni_equal(xjni_thread_t t1, xjni_thread_t t2) {
#ifdef _WIN32
	return GetThreadId(t1) == GetThreadId(t2);
#else
	return pthread_equal(t1,t2);
#endif
}

XJNI_THREAD_API xjni_thread_result_t XJNI_THREAD_CALL xjni_cancel(xjni_thread_t th) {
#ifdef _WIN32
	return ENOSYS;
#elif defined(__ANDROID__)
	return pthread_kill(th,SIGUSR1);
#else
	return pthread_cancel(th);
#endif
}

XJNI_THREAD_API int XJNI_THREAD_CALL xjni_setcanceltype(int type, int *oldtype) {
#ifdef _WIN32
	static XJNI_TLS int cancel_state = XJNI_CANCEL_ENABLE;
	if (oldtype) *oldtype = cancel_state;
	cancel_state = type;
	return 0;
#elif defined(__ANDROID__)
	static XJNI_TLS int cancel_type = XJNI_CANCEL_ENABLE;
	if (oldtype) *oldtype = cancel_type;
	cancel_type = type;
	if (type == XJNI_CANCEL_ENABLE) ENABLE_SIGNAL(SIGUSR1);
	else if (type == XJNI_CANCEL_DISABLE) DISABLE_SIGNAL(SIGUSR1);
	return 0;
#else
	return pthread_setcanceltype(type, oldtype);
#endif
}

XJNI_THREAD_API int XJNI_THREAD_CALL xjni_setcancelstate(int state, int *oldstate) {
	static XJNI_TLS int cancel_state = XJNI_CANCEL_ENABLE; // per-thread
#ifdef __ANDROID__
	if (oldstate != NULL) *(int*)oldstate = thread_cancel_state;
	if (state == XJNI_CANCEL_ENABLE) {
		thread_cancel_state = XJNI_CANCEL_ENABLE;
		ENABLE_SIGNAL(SIGUSR1);
	} else if (state == XJNI_CANCEL_DISABLE) {
		thread_cancel_state = XJNI_CANCEL_DISABLE;
		DISABLE_SIGNAL(SIGUSR1);
	}
#else
	if (oldstate) *oldstate = cancel_state;
	cancel_state = state;
#endif
	return 0;
}

XJNI_THREAD_API void XJNI_THREAD_CALL xjni_testcancel(void) {
#ifdef _WIN32
	return;
#else
	pthread_testcancel();
#endif
}
