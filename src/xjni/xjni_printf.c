#include <stdlib.h>

#include "base-jni.h"

#include <xjni_string.h>
#include <xjni_printf.h>

JNIEXPORTC int JNICALL vsjprintf(jchar * s,const jchar * format,va_list arg) {
	if (!s || !format) return -1;
	/* Convert UTF-16 format to UTF-8 */
	char *cformat = xjni_tostring(format);
	if (!cformat) return -1;
	/* ---- size query ---- */
	va_list aq;
	va_copy(aq,arg);

#ifdef _MSC_VER
	int needed = _vscprintf(cformat,aq);
#else
	int needed = vsnprintf(NULL,0,cformat,aq);
#endif
	va_end(aq);
	if (needed < 0) goto cleanup;
	/* +1 for '\0' */
	char *cbuffer = (char *)malloc((size_t)needed + 1);
	if (!cbuffer) goto cleanup;

	/* ---- actual formatting ---- */
	va_list ac;
	va_copy(ac,arg);
	int ret = -1;
	ret = vsnprintf(cbuffer,(size_t)needed + 1,cformat,ac);
	va_end(ac);
	if (ret < 0) goto cleanup;
	/* ---- UTF-8 → UTF-16 ---- */
	size_t outlen = (size_t)ret + 1;
	if (!xjni_fromstring(cbuffer,s,&outlen)) {
		ret = -1;
		goto cleanup;
	}
	ret = (int)outlen;

cleanup:
	free(cbuffer);
	free(cformat);
	return ret;
}

JNIEXPORTC int JNICALL sjprintf(jchar * s,const jchar * format,...) {
	if (!s || !format) return -1;
	va_list ap;
	va_start(ap,format);
	int ret = vsjprintf(s,format,ap);
	va_end(ap);
	return ret;
}

JNIEXPORTC int JNICALL vsnjprintf(jchar * s,size_t maxlen,const jchar * format,va_list arg) {
	if (!s || !format || maxlen <= 0) return -1;

	/* Convert format string */
	char *cformat = xjni_tostring(format);
	if (!cformat) return -1;

	/* Allocate temporary narrow buffer */
	char *cbuffer = (char *)malloc(maxlen);
	if (!cbuffer) goto cleanup;

	va_list ac;
	va_copy(ac,arg);

	int ret = -1;
#ifdef _MSC_VER
	ret = _vsnprintf(cbuffer,maxlen,cformat,ac);
	if (ret < 0 || (size_t)ret >= maxlen) {
		cbuffer[maxlen - 1] = '\0';
		ret = -1;
	}
#else
	ret = vsnprintf(cbuffer,maxlen,cformat,ac);
	if (ret < 0 || (size_t)ret >= maxlen) ret = -1;
#endif

	va_end(ac);

	if (ret >= 0) {
		/* Convert result back to UTF-16 */
		size_t outlen = maxlen;
		if (!xjni_fromstring(cbuffer,s,&outlen))
			ret = -1;
	}

cleanup:
	free(cbuffer);
	free(cformat);
	return ret;
}

JNIEXPORTC int JNICALL snjprintf(jchar * s,size_t maxlen,const jchar * format,...) {
	va_list ac;
	va_start(ac,format);
	int ret = vsnjprintf(s,maxlen,format,ac);
	va_end(ac);
	return ret;
}

JNIEXPORTC int JNICALL vjfprintf(FILE* stream,const jchar* format,va_list arg) {
	if (!stream || !format) return -1;
	char* cret = xjni_tostring(format);
	if (!cret) return -1;
	va_list ac;
	va_copy(ac,arg);
	int ret = vfprintf(stream,cret,ac);
	va_end(ac);
	free(cret);
	return ret;
}

JNIEXPORTC int JNICALL jfprintf(FILE* stream,const jchar* format,...) {
	va_list ac;
	va_start(ac,format);
	int ret = vjfprintf(stream,format,ac);
	va_end(ac);
	return ret;
}

JNIEXPORTC int JNICALL vjprintf(const jchar* format,va_list arg) {
	return vjfprintf(stdout,format,arg);
}

JNIEXPORTC int JNICALL jprintf(const jchar* format,...) {
	va_list ac;
	va_start(ac,format);
	int ret = vjprintf(format,ac);
	va_end(ac);
	return ret;
}

JNIEXPORTC int JNICALL vjdprintf(int fd,const jchar* format,va_list arg) {
	if (fd < 0 || !format) return -1;
	char* cret = xjni_tostring(format);
	if (!cret) return -1;
	va_list ac;
	va_copy(ac,arg);
	int ret = -1;
#ifdef _WIN32
	FILE* fp = fdopen(fd, "w");
	if (fp) {
		ret = vfprintf(fp, cret, ac);
		fflush(fp);
	} else {
		XJNI_LOGE("XJNIPrintf","fdopen failed for fd %d", fd);
	}
#else
	ret = vdprintf(fd,cret,ac);
#endif
	va_end(ac);
	free(cret);
	return ret;
}

JNIEXPORTC int JNICALL jdprintf(int fd,const jchar* format,...) {
	va_list ac;
	va_start(ac,format);
	int ret = vjdprintf(fd,format,ac);
	va_end(ac);
	return ret;
}
