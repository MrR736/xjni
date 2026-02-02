#include <xjni_log.h>

#ifdef __ANDROID__
#include <android/log.h>

static inline int toandroid_LogPriority(int prio) {
	switch (prio) {
		case XJNI_LOG_DEFAULT: return ANDROID_LOG_DEFAULT;
		case XJNI_LOG_VERBOSE: return ANDROID_LOG_VERBOSE;
		case XJNI_LOG_DEBUG: return ANDROID_LOG_DEBUG;
		case XJNI_LOG_INFO: return ANDROID_LOG_INFO;
		case XJNI_LOG_WARN: return ANDROID_LOG_WARN;
		case XJNI_LOG_ERROR: return ANDROID_LOG_ERROR;
		case XJNI_LOG_FATAL: return ANDROID_LOG_FATAL;
		case XJNI_LOG_SILENT: return ANDROID_LOG_SILENT;
		default: return ANDROID_LOG_UNKNOWN;
	}
}
#else
#include <stdio.h>
#include <time.h>
#include <string.h>

static inline const char* cbasename(const char* path) {
	const char *p1 = strrchr(path, '/');
	const char *p2 = strrchr(path, '\\');
	const char *p = p1 > p2 ? p1 : p2;
	return p ? p + 1 : path;
}

static inline const char* xjni_LogPrioritytostring(int prio) {
	switch (prio) {
		case XJNI_LOG_DEFAULT: return "Default";
		case XJNI_LOG_VERBOSE: return "Verbose";
		case XJNI_LOG_DEBUG: return "DEBUG";
		case XJNI_LOG_INFO: return "INFO";
		case XJNI_LOG_WARN: return "Warn";
		case XJNI_LOG_ERROR: return "ERROR";
		case XJNI_LOG_FATAL: return "FATAL";
		case XJNI_LOG_SILENT: return NULL;
		default: return "Unknown";
	}
}

#define XJNI_COLOR_RESET   "\x1b[0m"
#define XJNI_COLOR_RED     "\x1b[31m"
#define XJNI_COLOR_YELLOW  "\x1b[33m"
#define XJNI_COLOR_GREEN   "\x1b[32m"
#define XJNI_COLOR_BLUE    "\x1b[34m"
#define XJNI_COLOR_MAGENTA "\x1b[35m"
#define XJNI_COLOR_CYAN    "\x1b[36m"
#define XJNI_COLOR_WHITE   "\x1b[37m"

static const char* xjni_LogPriorityColor(int prio) {
	switch (prio) {
		case XJNI_LOG_VERBOSE:	return XJNI_COLOR_WHITE;
		case XJNI_LOG_DEBUG:	return XJNI_COLOR_CYAN;
		case XJNI_LOG_INFO:	return XJNI_COLOR_GREEN;
		case XJNI_LOG_WARN:	return XJNI_COLOR_YELLOW;
		case XJNI_LOG_ERROR:	return XJNI_COLOR_RED;
		case XJNI_LOG_FATAL:	return XJNI_COLOR_MAGENTA;
		default:		return XJNI_COLOR_RESET;
	}
}

int xjni_log_vccprint(int prio, int line, const char* file, const char* tag,const char* fmt, va_list ap) {
	va_list apc;
	va_copy(apc,ap);
	if (prio == XJNI_LOG_SILENT) {
		va_end(apc);
		return 0;
	}

	const char* t = tag ? tag : "XJNI";
	time_t now = time(NULL);
	struct tm tm;
	char ts[48];
#if defined(_WIN32)
	localtime_s(&tm, &now);
#else
	localtime_r(&now, &tm);
#endif
	strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", &tm);

	const char* color = xjni_LogPriorityColor(prio);

	// Print timestamp + priority + tag + file
	fprintf(stderr, "%s %s[%s]%s %s (%s:%d) ",
		ts,color,xjni_LogPrioritytostring(prio),
		XJNI_COLOR_RESET,t,cbasename(file),line);

	int ret = vfprintf(stderr, fmt, apc);
	va_end(apc);

	fputc('\n', stderr);
	return ret;
}

int xjni_log_ccprint(int prio,int line,const char* file,const char* tag,const char* fmt,...) {
	va_list ap;
	va_start(ap,fmt);
	int ret = xjni_log_vccprint(prio,line,file,tag,fmt,ap);
	va_end(ap);
	return ret;
}
#endif  // __ANDROID__


int xjni_log_vcprint(int prio,int line,const char* file,const char* tag,const char* fmt,va_list ap) {
	va_list apc;
	va_copy(apc,ap);
#ifdef __ANDROID__
	(void)file;
	(void)line;
	int ret = __android_log_vprint(toandroid_LogPriority(prio), tag, fmt, apc);
	va_end(apc);
	return ret;
#else
	if (prio == XJNI_LOG_SILENT) {
		va_end(apc);
		return 0;
	}

	const char* t = tag ? tag : XJNI_DEFAULT_TAG;
	time_t now = time(NULL);
	struct tm tm;
	char ts[48];
#if defined(_WIN32)
	localtime_s(&tm, &now);
#else
	localtime_r(&now, &tm);
#endif
	strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", &tm);
	fprintf(stderr, "%s ", ts);
	const char* pstr = xjni_LogPrioritytostring(prio);
	if (pstr) fprintf(stderr, "[%s] ", pstr);
	fprintf(stderr, "%s ", t);
	if (file && line > 0) fprintf(stderr, "(%s:%d) ", cbasename(file), line);
	int ret = vfprintf(stderr, fmt, apc);
	va_end(apc);
	fputc('\n', stderr);
	fflush(stderr);
	return ret;
#endif
}

int xjni_log_cprint(int prio,int line,const char* file,const char* tag,const char* fmt,...) {
	va_list ap;
	va_start(ap,fmt);
	int ret = xjni_log_vcprint(prio,line,file,tag,fmt,ap);
	va_end(ap);
	return ret;
}
