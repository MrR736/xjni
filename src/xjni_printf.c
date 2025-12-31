#include <stdio.h>
#include <stdlib.h>
#include <jni.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni.h>

JNIEXPORTC int JNICALL vsjprintf(jchar * __s,const jchar * __format,va_list __arg) {
	if (!__s || !__format)
		return -1;

	int ret = -1;
	char *cformat = NULL;
	char *cbuffer = NULL;

	/* Convert UTF-16 format to UTF-8 */
	cformat = xjni_tostring(__format);
	if (!cformat)
		return -1;

	/* ---- size query ---- */
	va_list aq;
	va_copy(aq, __arg);

#ifdef _MSC_VER
	int needed = _vscprintf(cformat, aq);
#else
	int needed = vsnprintf(NULL, 0, cformat, aq);
#endif

	va_end(aq);

	if (needed < 0)
		goto cleanup;

	/* +1 for '\0' */
	cbuffer = (char *)malloc((size_t)needed + 1);
	if (!cbuffer)
		goto cleanup;

	/* ---- actual formatting ---- */
	va_list ac;
	va_copy(ac, __arg);
	ret = vsnprintf(cbuffer, (size_t)needed + 1, cformat, ac);
	va_end(ac);

	if (ret < 0)
		goto cleanup;

	/* ---- UTF-8 → UTF-16 ---- */
	size_t outlen = (size_t)ret + 1;
	if (!xjni_fromstring(cbuffer, __s, &outlen)) {
		ret = -1;
		goto cleanup;
	}

	ret = (int)outlen;

cleanup:
	free(cbuffer);
	free(cformat);
	return ret;
}

JNIEXPORTC int JNICALL sjprintf(jchar * __s, const jchar * __format,...) {
	if (!__s || !__format)
		return -1;

	va_list ap;
	va_start(ap, __format);
	int ret = vsjprintf(__s, __format, ap);
	va_end(ap);

	return ret;
}

JNIEXPORTC int JNICALL vsnjprintf(jchar * __s,size_t __maxlen,const jchar * __format,va_list __arg) {
	if (!__s || !__format || __maxlen <= 0)
		return -1;

	int ret = -1;
	char *cformat = NULL;
	char *cbuffer = NULL;

	/* Convert format string */
	cformat = xjni_tostring(__format);
	if (!cformat)
		return -1;

	/* Allocate temporary narrow buffer */
	cbuffer = (char *)malloc(__maxlen);
	if (!cbuffer)
		goto cleanup;

	va_list ac;
	va_copy(ac, __arg);

#ifdef _MSC_VER
	ret = _vsnprintf(cbuffer, __maxlen, cformat, ac);
	if (ret < 0 || (size_t)ret >= __maxlen) {
		cbuffer[__maxlen - 1] = '\0';
		ret = -1;
	}
#else
	ret = vsnprintf(cbuffer, __maxlen, cformat, ac);
	if (ret < 0 || (size_t)ret >= __maxlen)
		ret = -1;
#endif

	va_end(ac);

	if (ret >= 0) {
		/* Convert result back to UTF-16 */
		size_t outlen = __maxlen;
		if (!xjni_fromstring(cbuffer, __s, &outlen))
			ret = -1;
	}

cleanup:
	free(cbuffer);
	free(cformat);
	return ret;
}

JNIEXPORTC int JNICALL snjprintf(jchar * __s,size_t __maxlen,const jchar * __format, ...) {
	va_list ac;
	va_start(ac,__format);
	int ret = vsnjprintf(__s,__maxlen,__format, ac);
	va_end(ac);
	return ret;
}

JNIEXPORTC int JNICALL vjfprintf(FILE* __stream, const jchar* __format, va_list __arg) {
	if (!__stream || !__format) return -1;
	char* cret = xjni_tostring(__format);
	if (!cret) return -1;
	va_list ac;
	va_copy(ac, __arg);
	int ret = vfprintf(__stream, cret, ac);
	va_end(ac);
	free(cret);
	return ret;
}

JNIEXPORTC int JNICALL jfprintf(FILE* __stream, const jchar* __format, ...) {
	va_list ac;
	va_start(ac,__format);
	int ret = vjfprintf(__stream, __format, ac);
	va_end(ac);
	return ret;
}

JNIEXPORTC int JNICALL vjprintf(const jchar* __format, va_list __arg) {
	return vjfprintf(stdout,__format,__arg);
}

JNIEXPORTC int JNICALL jprintf(const jchar* __format, ...) {
	va_list ac;
	va_start(ac,__format);
	int ret = vjprintf(__format, ac);
	va_end(ac);
	return ret;
}

JNIEXPORTC int JNICALL vjdprintf(int __fd, const jchar* __format, va_list __arg) {
	if (__fd < 0 || !__format) return -1;
	char* cret = xjni_tostring(__format);
	if (!cret) return -1;
	va_list ac;
	va_copy(ac, __arg);
	int ret = vdprintf(__fd, cret, ac);
	va_end(ac);
	free(cret);
	return ret;
}

JNIEXPORTC int JNICALL jdprintf(int __fd, const jchar* __format, ...) {
	va_list ac;
	va_start(ac,__format);
	int ret = vjdprintf(__fd, __format, ac);
	va_end(ac);
	return ret;
}
