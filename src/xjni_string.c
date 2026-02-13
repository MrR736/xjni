#include <stdlib.h>

#define LOG_TAG "xjni"
#include "base-jni.h"

#include <xjni_string.h>

#ifdef _WIN32
#include <windows.h>
#endif

JNIEXPORTC jint JNICALL xjni_tochar(const jchar c,char* out,size_t out_size) {
	 if (!out || out_size == 0) return 0;

	 if (c <= 0x7F) {
		out[0] = (char)c;
		return 1;
	 } else if (c <= 0x7FF) {
		if (out_size < 2) return 0;
		out[0] = (char)(0xC0 | ((c >> 6) & 0x1F));
		out[1] = (char)(0x80 | (c & 0x3F));
		return 2;
	 } else {
		if (out_size < 3) return 0;
		out[0] = (char)(0xE0 | ((c >> 12) & 0x0F));
		out[1] = (char)(0x80 | ((c >> 6) & 0x3F));
		out[2] = (char)(0x80 | (c & 0x3F));
		return 3;
	 }
}

JNIEXPORTC jint JNICALL xjni_tojchar(const char* utf8,size_t utf8_len,jchar* out) {
	 if (!utf8 || !out || utf8_len == 0) return 0;

	 unsigned char c0 = (unsigned char)utf8[0];

	 if (c0 <= 0x7F) {
		  // 1-byte ASCII
		  *out = (jchar)c0;
		  return 1;
	 } else if ((c0 & 0xE0) == 0xC0 && utf8_len >= 2) {
		  // 2-byte UTF-8
		  unsigned char c1 = (unsigned char)utf8[1];
		  if ((c1 & 0xC0) != 0x80) return 0; // invalid
		  *out = (jchar)(((c0 & 0x1F) << 6) | (c1 & 0x3F));
		  return 2;
	 } else if ((c0 & 0xF0) == 0xE0 && utf8_len >= 3) {
		  // 3-byte UTF-8
		  unsigned char c1 = (unsigned char)utf8[1];
		  unsigned char c2 = (unsigned char)utf8[2];
		  if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80) return 0; // invalid
		  *out = (jchar)(((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F));
		  return 3;
	 }
	 return 0;
}

JNIEXPORTC char* JNICALL xjni_tostring(const jchar* c) {
	if (!c) return NULL;

	size_t max_len = 0;
	for (size_t i = 0; c[i]; i++) {
		max_len += 3;
		if (c[i] >= 0xD800 && c[i] <= 0xDBFF) {
			max_len += 1;
			i++;
		}
	}
	char* out = (char*)malloc(max_len + 1);
	if (!out) return NULL;
	char* p = out;
	for (size_t i = 0; c[i]; i++) {
		uint32_t ch = c[i];
		if (ch <= 0x7F) {
			*p++ = ch;
		} else if (ch <= 0x7FF) {
			*p++ = 0xC0 | (ch >> 6);
			*p++ = 0x80 | (ch & 0x3F);
		} else if (ch >= 0xD800 && ch <= 0xDBFF) {
			uint32_t low = c[++i];
			uint32_t codepoint = 0x10000 + (((ch - 0xD800) << 10) | (low - 0xDC00));
			*p++ = 0xF0 | (codepoint >> 18);
			*p++ = 0x80 | ((codepoint >> 12) & 0x3F);
			*p++ = 0x80 | ((codepoint >> 6) & 0x3F);
			*p++ = 0x80 | (codepoint & 0x3F);
		} else {
			*p++ = 0xE0 | (ch >> 12);
			*p++ = 0x80 | ((ch >> 6) & 0x3F);
			*p++ = 0x80 | (ch & 0x3F);
		}
	}
	*p = '\0';
	return out;
}

JNIEXPORTC jchar* JNICALL xjni_tojstring(const char* c) {
	if (!c) return NULL;

	size_t len = 0;
	for (size_t i = 0; c[i]; ) {
		unsigned char b = c[i];
		if (b <= 0x7F) {
			i += 1; len += 1;
		} else if ((b & 0xE0) == 0xC0) {
			i += 2; len += 1;
		} else if ((b & 0xF0) == 0xE0) {
			i += 3; len += 1;
		} else if ((b & 0xF8) == 0xF0) {
			i += 4; len += 2;
		} else {
			i += 1; len += 1;
		}
	}

	jchar* out = (jchar*)malloc((len + 1) * sizeof(jchar));
	if (!out) return NULL;

	size_t j = 0;
	for (size_t i = 0; c[i]; ) {
		uint32_t codepoint = 0;
		unsigned char b = c[i];
		if (b <= 0x7F) {
			codepoint = b; i += 1;
		} else if ((b & 0xE0) == 0xC0) {
			codepoint = ((c[i] & 0x1F) << 6) | (c[i+1] & 0x3F);
			i += 2;
		} else if ((b & 0xF0) == 0xE0) {
			codepoint = ((c[i] & 0x0F) << 12) | ((c[i+1] & 0x3F) << 6) | (c[i+2] & 0x3F);
			i += 3;
		} else if ((b & 0xF8) == 0xF0) {
			codepoint = ((c[i] & 0x07) << 18) | ((c[i+1] & 0x3F) << 12) | ((c[i+2] & 0x3F) << 6) | (c[i+3] & 0x3F);
			i += 4;
			codepoint -= 0x10000;
			out[j++] = 0xD800 | (codepoint >> 10);
			out[j++] = 0xDC00 | (codepoint & 0x3FF);
			continue;
		}
		out[j++] = (jchar)codepoint;
	}
	out[j] = 0;
	return out;
}

JNIEXPORTC jboolean JNICALL xjni_fromstring(const char *src,jchar *dst,size_t *dstlen) {
	if (!src || !dst || !dstlen || *dstlen == 0)
		return JNI_FALSE;

	size_t cap = *dstlen;
	size_t out = 0;

	const unsigned char *s = (const unsigned char*)src;

	while (*s && out + 1 < cap) {
		uint32_t codepoint;

		/* -------- UTF-8 decode -------- */
		if (*s < 0x80) {
			codepoint = *s++;
		} else if ((*s & 0xE0) == 0xC0) {
			if ((s[1] & 0xC0) != 0x80)
				return JNI_FALSE;
			codepoint = ((*s & 0x1F) << 6) | (s[1] & 0x3F);
			s += 2;
		} else if ((*s & 0xF0) == 0xE0) {
			if ((s[1] & 0xC0) != 0x80 ||
				(s[2] & 0xC0) != 0x80)
				return JNI_FALSE;
			codepoint = ((*s & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
			s += 3;
		} else if ((*s & 0xF8) == 0xF0) {
			if ((s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80 || (s[3] & 0xC0) != 0x80)
				return JNI_FALSE;
			codepoint = ((*s & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);
			s += 4;
		} else {
			return JNI_FALSE;
		}

		/* -------- UTF-16 encode -------- */
		if (codepoint <= 0xFFFF) {
			if (codepoint >= 0xD800 && codepoint <= 0xDFFF)
				return JNI_FALSE; /* surrogate invalid */
			dst[out++] = (jchar)codepoint;
		} else if (codepoint <= 0x10FFFF) {
			if (out + 2 >= cap)
				break;
			codepoint -= 0x10000;
			dst[out++] = (jchar)(0xD800 | (codepoint >> 10));
			dst[out++] = (jchar)(0xDC00 | (codepoint & 0x3FF));
		} else {
			return JNI_FALSE;
		}
	}

	dst[out] = 0;
	*dstlen = out;

	return (*s == '\0');
}

JNIEXPORTC jboolean JNICALL xjni_fromjstring(const jchar *src,char *dst,size_t *dstlen) {
	if (!src || !dst || !dstlen || *dstlen == 0)
		return JNI_FALSE;

	size_t cap = *dstlen;
	size_t out = 0;

	while (*src) {
		uint32_t codepoint;

		/* -------- UTF-16 decode -------- */
		jchar wc = *src++;

		if (wc >= 0xD800 && wc <= 0xDBFF) {
			/* High surrogate */
			jchar wc2 = *src++;
			if (wc2 < 0xDC00 || wc2 > 0xDFFF)
				return JNI_FALSE;
			codepoint = 0x10000 +
						(((wc - 0xD800) << 10) |
						 (wc2 - 0xDC00));
		} else if (wc >= 0xDC00 && wc <= 0xDFFF) {
			return JNI_FALSE;
		} else {
			codepoint = wc;
		}

		/* -------- UTF-8 encode -------- */
		if (codepoint <= 0x7F) {
			if (out + 1 >= cap) break;
			dst[out++] = (char)codepoint;
		} else if (codepoint <= 0x7FF) {
			if (out + 2 >= cap) break;
			dst[out++] = (char)(0xC0 | (codepoint >> 6));
			dst[out++] = (char)(0x80 | (codepoint & 0x3F));
		} else if (codepoint <= 0xFFFF) {
			if (out + 3 >= cap) break;
			dst[out++] = (char)(0xE0 | (codepoint >> 12));
			dst[out++] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
			dst[out++] = (char)(0x80 | (codepoint & 0x3F));
		} else if (codepoint <= 0x10FFFF) {
			if (out + 4 >= cap) break;
			dst[out++] = (char)(0xF0 | (codepoint >> 18));
			dst[out++] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
			dst[out++] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
			dst[out++] = (char)(0x80 | (codepoint & 0x3F));
		} else {
			return JNI_FALSE;
		}
	}

	dst[out] = '\0';
	*dstlen = out;

	return (*src == 0);
}

JNIEXPORTC void* JNICALL jmemcpy(void *dest,const void *src,size_t n) {
	unsigned char *tmp = (unsigned char*)dest;
	const unsigned char *s = (const unsigned char*)src;
	while (n--) *tmp++ = *s++;
	return dest;
}

JNIEXPORTC void* JNICALL jmemmove(void *dest,const void *src,size_t n) {
	unsigned char *tmp;
	unsigned char *s;
	if (dest <= src) {
		tmp = (unsigned char*)dest;
		s = (unsigned char*)src;
		while (n--) *tmp++ = *s++;
	} else {
		tmp = (unsigned char*)dest + n;
		s = (unsigned char*)src + n;
		while (n--) *--tmp = *--s;
	}
	return dest;
}

JNIEXPORTC void* JNICALL jmemchr(const void *s,jint c,size_t n) {
	const unsigned char *p = s;
	while (n--) { if (base_cast(unsigned char,c) == *p++) return ubase_cast(void*,p - 1); }
	return NULL;
}

JNIEXPORTC void* JNICALL jmemset(void *s,jint c,size_t n) {
	unsigned char *xs = (unsigned char*)s;
	while (n--) *xs++ = (unsigned char)c;
	return s;
}

JNIEXPORTC jint JNICALL jmemcmp(const void *cs,const void *ct,size_t count) {
	const unsigned char *su1,*su2;
	jint res = 0;
	for(su1 = cs,su2 = ct; 0 < count; ++su1,++su2,count--) { if ((res = *su1 - *su2) != 0) break; }
	return res;
}

JNIEXPORTC size_t JNICALL jstrlen(const jchar* __s) {
	if (__s == NULL) return 0;
	const jchar *p = __s;
	while (*p) ++p;
	return base_cast(size_t,p - __s);
}

JNIEXPORTC size_t JNICALL jstrnlen(const jchar * s,size_t count) {
	const jchar *sc;
	for (sc = s; count-- && *sc != '\0'; ++sc)
		/* nothing */;
	return base_cast(jint,sc - s);
}

JNIEXPORTC jchar* JNICALL jstrrchr(const jchar *s,jint c) {
	const jchar *p = s + jstrlen(s);
	for (; *p != (jchar)c; --p) { if (p == s) return NULL; }
	return (jchar *)p;
}

JNIEXPORTC jchar* JNICALL jstrchr(const jchar *s,jint c) {
	for(; *s != (jchar)c; ++s) { if (*s == '\0') return NULL; }
	return (jchar *)s;
}

JNIEXPORTC jint JNICALL jstrcmp(const jchar *cs,const jchar *ct) {
	unsigned char *csu = (unsigned char*)cs;
	unsigned char *ctu = (unsigned char*)ct;
	while (*csu && (*csu == *ctu)) { csu++; ctu++; }
	return base_cast(jint,*csu - *ctu);
}

JNIEXPORTC size_t JNICALL jstrlcpy(jchar *dest,const jchar *src,size_t size) {
	size_t ret = jstrlen(src);
	if (size) {
		size_t len = (ret >= size) ? size - 1 : ret;
		jmemcpy(dest,src,len);
		dest[len] = '\0';
	}
	return ret;
}

JNIEXPORTC jchar* JNICALL jstrcpy(jchar* __dest,const jchar* __src) {
	if (__dest == NULL || __src == NULL) return NULL;
	jchar* dest = __dest;
	const jchar* src = __src;
	while (*src != 0) { *dest = *src; ++dest; ++src; }
	*dest = '\0';
	return __dest;
}


JNIEXPORTC jint JNICALL jstrncmp(const jchar *cs,const jchar *ct,size_t count) {
	unsigned char *csu = (unsigned char*)cs;
	unsigned char *ctu = (unsigned char*)ct;
	jint res = 0;
	while (count) {
		if ((res = *csu - *ctu++) != 0 || !*csu++) break;
		count--;
	}
	return res;
}

JNIEXPORTC size_t JNICALL jstrlcat(jchar *dest,const jchar *src,size_t size) {
	size_t slen = jstrlen(src);
	size_t dlen = jstrnlen(dest,size);
	jchar *p = dest + dlen;

	while ((p - dest) < size)
		if ((*p++ = *src++) == '\0')
			break;

	if (dlen < size)
		*(p-1) = '\0';

	return slen + dlen;
}

JNIEXPORTC jchar* JNICALL jstrcat(jchar* __dest,const jchar* __src) {
	size_t dlen = jstrlen(__dest);
	jchar* p = __dest + dlen;
	while (*__src != '\0') { *p++ = *__src++; }
	*p = '\0';
	return __dest;
}

JNIEXPORTC jchar* JNICALL jstrpbrk(const jchar * cs,const jchar * ct) {
	const jchar *sc1,*sc2;
	for(sc1 = cs; *sc1 != '\0'; ++sc1) {
		for(sc2 = ct; *sc2 != '\0'; ++sc2) {
			if (*sc1 == *sc2) return (jchar*)sc1;
		}
	}
	return NULL;
}

JNIEXPORTC jchar* JNICALL jstrstr(const jchar *s1,const jchar *s2) {
	size_t l1 = jstrlen(s1);
	size_t l2 = jstrlen(s2);

	if (l2 == 0) return (jchar*)s1;

	for (const jchar *p = s1; *p != '\0'; ++p) {
		if (l1 < l2) return NULL;

		if (jmemcmp(p,s2,l2 * sizeof(jchar)) == 0) return (jchar*)p;
	}
	return NULL;
}

JNIEXPORTC size_t JNICALL jstrspn(const jchar *s,const jchar *accept) {
	const jchar *p = s;
	const jchar *a;
	size_t count = 0;
	for (; *p != '\0'; ++p) {
		jint found = 0;
		for (a = accept; *a != '\0'; ++a) {
			if (*p == *a) {
				found = 1;
				break;
			}
		}
		if (!found)
			return count;
		++count;
	}
	return count;
}

JNIEXPORTC jchar* JNICALL jstrdup(const jchar *s) {
	size_t l = jstrlen(s);
	jchar *tmp = (jchar*)malloc((l + 1) * sizeof(jchar));
	if (tmp == NULL) return NULL;
	for (size_t i = 0; i <= l; i++) tmp[i] = s[i];
	return tmp;
}

JNIEXPORTC jchar* JNICALL jstrndup(const jchar *__string,size_t __n) {
	jchar *tmp = (jchar*)malloc((__n + 1) * sizeof(jchar));
	if (tmp == NULL) return NULL;
	for (size_t i = 0; i < __n; i++) { tmp[i] = __string[i]; }
	tmp[__n] = '\0';
	return tmp;
}

JNIEXPORTC size_t JNICALL jstrcspn(const jchar *__s,const jchar *__reject) {
	size_t i;
	for (i = 0; __s[i] != '\0'; i++) {
		for (size_t j = 0; __reject[j] != '\0'; j++) {
			if (__s[i] == __reject[j]) return i;
		}
	}
	return i;
}

JNIEXPORTC jchar* JNICALL jstrchrnul(const jchar *__s,jint __c) {
	while (*__s != '\0') {
		if (*__s == (jchar)__c)
			return (jchar *)__s;
		__s++;
	}
	return (jchar*)__s;
}


JNIEXPORTC jint JNICALL jstrcoll(const jchar *__s1,const jchar *__s2) {
	while (*__s1 && *__s2) {
		if (*__s1 != *__s2) return (*__s1 < *__s2) ? -1 : 1;
		__s1++; __s2++;
	}
	if (*__s1 == '\0' && *__s2 == '\0') return 0;
	return (*__s1 == '\0') ? -1 : 1;
}

JNIEXPORTC size_t JNICALL jstrxfrm(jchar* __dest,const jchar* __src,size_t __n) {
	if (__src == NULL || __dest == NULL) return 0;
	size_t src_len = jstrlen(__src);
	size_t copy_len = (src_len < (__n - 1)) ? src_len : (__n - 1);
	for (size_t i = 0; i < copy_len; i++) { __dest[i] = __src[i]; }
	__dest[copy_len] = '\0';
	return copy_len;
}

JNIEXPORTC jchar* JNICALL jstrtok(jchar* __s,const jchar* __delim) {
	static jchar* last = NULL;
	if (__s != NULL) last = __s;
	if (last == NULL || *last == '\0') return NULL;
	while (*last != '\0' && jstrchr(__delim,*last)) last++;
	if (*last == '\0') return NULL;
	jchar* start = last;
	while (*last != '\0' && !jstrchr(__delim,*last)) last++;
	if (*last != '\0') { *last = '\0'; last++; }
	return start;
}

JNIEXPORTC void JNICALL jstrreverse(jchar* __str) {
	size_t len = jstrlen(__str);
	jchar *tmp = (jchar*)malloc((len + 1) * sizeof(jchar));

	for (size_t i = 0; i < len / 2; ++i) {
		jchar temp = __str[i];
		__str[i] = __str[len - i - 1];
		__str[len - i - 1] = temp;
	}
}

JNIEXPORT jchar* JNICALL jstrrev(const jchar* jstr) {
	if (!jstr) return NULL;
	size_t len = jstrlen(jstr);
	jchar *tmp = (jchar*)malloc((len + 1) * sizeof(jchar));
	if (!tmp) return NULL;
	for (size_t i = 0; i < len; ++i) tmp[i] = jstr[len - 1 - i];
	tmp[len] = '\0';
	return tmp;
}

JNIEXPORTC char* JNICALL xjni_wcstombs(const wchar_t *wcs) {
	if (!wcs) return NULL;
#ifdef _WIN32
	int len = WideCharToMultiByte(CP_UTF8,0,wcs,-1,NULL,0,NULL,NULL);
	if (len <= 0) return NULL;
	char *mbs = (char*)malloc((size_t)len);
	if (!mbs) return NULL;
	WideCharToMultiByte(CP_UTF8,0,wcs,-1,mbs,len,NULL,NULL);
	return mbs;
#else
	size_t len = wcstombs(NULL,wcs,0);
	if (len <= 0) return NULL;
	char *mbs = (char*)malloc(len + 1);
	if (!mbs) return NULL;
	wcstombs(mbs,wcs,len + 1);
	return mbs;
#endif
}

JNIEXPORTC wchar_t* JNICALL xjni_mbstowcs(const char *mbs) {
	if (!mbs) return NULL;
#ifdef _WIN32
	int len = MultiByteToWideChar(CP_UTF8,0,mbs,-1,NULL,0);
	if (len <= 0) return NULL;
	wchar_t *wcs = (wchar_t*)malloc(len * sizeof(wchar_t));
	if (!wcs) return NULL;
	MultiByteToWideChar(CP_UTF8,0,mbs,-1,wcs,len);
	return wcs;
#else
	size_t len = mbstowcs(NULL,mbs,0);
	if (len == (size_t)-1) return NULL;
	wchar_t *wcs = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	if (!wcs) return NULL;
	mbstowcs(wcs,mbs,len + 1);
	return wcs;
#endif
}

JNIEXPORTC wchar_t* JNICALL xjni_jmbstowcs(const jchar* c) {
	if (!c) return NULL;
	char* ret = xjni_tostring(c);
	if (!ret) return NULL;
	wchar_t* wret = xjni_mbstowcs(ret);
	if (!wret) {
		free(ret);
		return NULL;
	}
	free(ret);
	return wret;
}

JNIEXPORTC jchar* JNICALL xjni_jwcstombs(const wchar_t* c) {
	if (!c) return NULL;
	char* ret = xjni_wcstombs(c);      // wchar_t → UTF-8
	if (!ret) return NULL;
	jchar* jret = xjni_tojstring(ret); // UTF-8 → UTF-16
	if (!jret) {
		free(ret);
		return NULL;
	}
	free(ret);
	return jret;
}
