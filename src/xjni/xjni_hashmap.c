/**
 * xjni_hashmap.c : Extern JNI HashMap Utility
 *
 * Copyright (C) 2025 MrR736
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdlib.h>

#include <xjnidef.h>
#include <xjni_string.h>
#include <xjni_hashmap.h>

#define MAX_LOAD_FACTOR		0.70f
#define TOMBSTONE_CLEAN_RATIO	0.30f
#define RESIZE_GROW_FACTOR	2

static inline size_t hash(const char* key, size_t cap) {
	size_t h = 2166136261u;
	while (*key) {
		h ^= (unsigned char)*key++;
		h *= 16777619u;
	}
	return h % cap;
}

/**
 * Safe string comparison
 * Returns 0 if equal, <0 if cs < ct, >0 if cs > ct.
 * NULL is considered less than any non-NULL string.
 */
#ifdef __XJNI_MODERN_CPU__
static inline int XJNI_strcmp(const char *cs, const char *ct) {
	if (cs == ct) return 0;
	if (!cs) return -1;
	if (!ct) return 1;

	const unsigned char *s1 = (const unsigned char *)cs;
	const unsigned char *s2 = (const unsigned char *)ct;

	// Align to word boundary
	while (((uintptr_t)s1 & (sizeof(uint64_t)-1)) &&
		   *s1 && (*s1 == *s2)) {
		s1++; s2++;
	}

	const uint64_t *w1 = (const uint64_t *)s1;
	const uint64_t *w2 = (const uint64_t *)s2;

	while (*w1 == *w2) {
		// detect zero byte in word
		uint64_t v = *w1;
		if (((v - 0x0101010101010101ULL) & ~v & 0x8080808080808080ULL))
			return 0; // equal and terminated

		w1++; w2++;
	}

	// fallback to byte compare
	s1 = (const unsigned char *)w1;
	s2 = (const unsigned char *)w2;

	while (*s1 && (*s1 == *s2)) {
		s1++; s2++;
	}

	return (int)*s1 - (int)*s2;
}
#else
static inline int XJNI_strcmp(const char *cs, const char *ct) {
	if (cs == ct) return 0;
	if (!cs) return -1;
	if (!ct) return 1;

	const unsigned char *s1 = (const unsigned char *)cs;
	const unsigned char *s2 = (const unsigned char *)ct;

	for (;;) {
		unsigned char c1 = *s1++;
		unsigned char c2 = *s2++;

		if (c1 != c2)
			return (int)c1 - (int)c2;

		if (c1 == '\0')
			return 0;
	}
}
#endif


/**
 * Safe string length
 * Returns 0 if the string is NULL
 */
#ifdef __XJNI_MODERN_CPU__
static inline size_t XJNI_strlen(const char *s) {
	if (!s) return 0;
	const char *p = s;
	while ((uintptr_t)p & (sizeof(uintptr_t) - 1)) {
		if (*p == '\0') return (size_t)(p - s);
		++p;
	}
	const uintptr_t *wp = (const uintptr_t *)p;
	for (;;) {
		uintptr_t v = *wp;
		if ((v - 0x0101010101010101ULL) & ~v & 0x8080808080808080ULL) {
			p = (const char *)wp;
			while (*p) ++p;
			return (size_t)(p - s);
		}
		++wp;
	}
}
#else
static inline size_t XJNI_strlen(const char *s) {
	if (!s) return 0;
	const char *p = s;
	for (;;) {
		if (*p == '\0') return (size_t)(p - s);
		++p;
	}
}
#endif

/**
 * Safe string copy with truncation
 * Copies at most size-1 characters to dest and always null-terminates.
 * Returns the length of src (like strlcpy), even if truncated.
 */
#ifdef __XJNI_MODERN_CPU__
static inline size_t XJNI_strncpy(char *dest, const char *src, size_t size) {
	if (!dest || !src) return 0;
	size_t len = XJNI_strlen(src);
	if (size) {
		size_t copy = (len >= size) ? size - 1 : len;
		jmemcpy(dest, src, copy);
		dest[copy] = '\0';
	}
	return len;
}
#else
static inline size_t XJNI_strncpy(char *dest, const char *src, size_t size) {
	if (!dest || !src) return 0;
	const char *s = src;
	size_t n = size;
	if (n != 0) {
		while (--n != 0) {
			char c = *s++;
			*dest++ = c;
			if (c == '\0') return (size_t)(s - src - 1);
		}
		*dest = '\0';
	}
	while (*s++);
	return (size_t)(s - src - 1);
}
#endif

int XJNI_HashMapInitNoMutex(XJNI_HashMap* map, size_t capacity) {
	if (!map || capacity == 0) return 0;
	jmemset(map, 0, sizeof(*map));
	map->table_size = capacity;
	map->hash = calloc(capacity, sizeof(XJNI_HashMapKeyValue));
	if (!map->hash) return 0;
	return 1;
}

int XJNI_HashMapInit(XJNI_HashMap* map, size_t capacity) {
	int ret = XJNI_HashMapInitNoMutex(map,capacity);
	xjni_mutex_init(&map->mutex);
	return ret;
}

static void XJNI_HashMapInsertRaw(XJNI_HashMap* map, const char* key, XJNI_HashValue_t value) {
	size_t index = hash(key, map->table_size);
	size_t firstTombstone = (size_t)-1;

	while (map->hash[index].state != XJNI_SLOT_EMPTY) {
		if (map->hash[index].state == XJNI_SLOT_OCCUPIED && XJNI_strcmp(map->hash[index].key, key) == 0) {
			map->hash[index].value = value;
			return;
		}
		if (map->hash[index].state == XJNI_SLOT_TOMBSTONE && firstTombstone == (size_t)-1) {
			firstTombstone = index;
		}
		index = (index + 1) % map->table_size;
	}

	size_t target = (firstTombstone != (size_t)-1) ? firstTombstone : index;
	if (map->hash[target].state == XJNI_SLOT_TOMBSTONE) map->tombstones--;
	XJNI_strncpy(map->hash[target].key,key,sizeof(map->hash[target].key));
	map->hash[target].value = value;
	map->hash[target].state = XJNI_SLOT_OCCUPIED;
	map->size++;
}

static void XJNI_HashMapRehash(XJNI_HashMap* map, size_t newCap) {
	XJNI_HashMapKeyValue* old = map->hash;
	size_t oldCap = map->table_size;
	size_t targetCap = (size_t)(map->table_size * RESIZE_GROW_FACTOR);
	if (targetCap < newCap) targetCap = newCap;

	XJNI_HashMapKeyValue* newTable = calloc(targetCap, sizeof(*newTable));
	if (!newTable) return;

	map->hash = newTable;
	map->table_size = targetCap;
	map->size = 0;
	map->tombstones = 0;

	for (size_t i = 0; i < oldCap; i++) {
		if (old[i].state == XJNI_SLOT_OCCUPIED) XJNI_HashMapInsertRaw(map, old[i].key, old[i].value);
	}
	free(old);
}

void XJNI_HashMapInsertNoLock(XJNI_HashMap* map, const char* key, XJNI_HashValue_t value) {
	if (!map || !key) return;
	float load = (float)(map->size + map->tombstones) / map->table_size;
	float dead = (float)map->tombstones / map->table_size;

	if (load >= MAX_LOAD_FACTOR || dead >= TOMBSTONE_CLEAN_RATIO)
		XJNI_HashMapRehash(map, map->table_size * RESIZE_GROW_FACTOR);

	XJNI_HashMapInsertRaw(map, key, value);
}

void XJNI_HashMapInsert(XJNI_HashMap* map, const char* key, XJNI_HashValue_t value) {
	if (!map || !key) return;
	xjni_mutex_lock(&map->mutex);
	XJNI_HashMapInsertNoLock(map, key, value);
	xjni_mutex_unlock(&map->mutex);
}

int XJNI_HashMapTryGetNoLock(XJNI_HashMap* map, const char* key,XJNI_HashValue_t* out) {
	if (!map || !key || !map->hash) return 0;

	size_t index = hash(key, map->table_size);
	size_t start = index;

	while (map->hash[index].state != XJNI_SLOT_EMPTY) {
		if (map->hash[index].state == XJNI_SLOT_OCCUPIED && XJNI_strcmp(map->hash[index].key, key) == 0) {
			if (out) *out = map->hash[index].value;
			return 1;
		}
		index = (index + 1) % map->table_size;
		if (index == start) break;
	}
	return 0;
}

int XJNI_HashMapTryGet(XJNI_HashMap* map, const char* key,XJNI_HashValue_t* out) {
	xjni_mutex_lock(&map->mutex);
	int result = XJNI_HashMapTryGetNoLock(map, key, out);
	xjni_mutex_unlock(&map->mutex);
	return result;
}

XJNI_HashValue_t XJNI_HashMapSearchNoLock(XJNI_HashMap* map, const char* key) {
	if (!map || !key) return XJNI_HASHMAP_NOT_FOUND;
	XJNI_HashValue_t v;
	int found = XJNI_HashMapTryGetNoLock(map, key, &v);
	return found ? v : XJNI_HASHMAP_NOT_FOUND;
}

XJNI_HashValue_t XJNI_HashMapSearch(XJNI_HashMap* map, const char* key) {
	if (!map || !key) return XJNI_HASHMAP_NOT_FOUND;
	XJNI_HashValue_t v;
	int found = XJNI_HashMapTryGet(map, key, &v);
	return found ? v : XJNI_HASHMAP_NOT_FOUND;
}

void XJNI_HashMapDeleteNoLock(XJNI_HashMap* map, const char* key) {
	if (!map || !key) return;
	size_t index = hash(key, map->table_size);
	size_t start = index;
	while (map->hash[index].state != XJNI_SLOT_EMPTY) {
		if (map->hash[index].state == XJNI_SLOT_OCCUPIED && XJNI_strcmp(map->hash[index].key, key) == 0) {
			map->hash[index].state = XJNI_SLOT_TOMBSTONE;
			map->size--;
			map->tombstones++;
			break;
		}
		index = (index + 1) % map->table_size;
		if (index == start) break;
	}
	if ((float)map->tombstones / map->table_size >= TOMBSTONE_CLEAN_RATIO)
		XJNI_HashMapRehash(map, map->table_size);
}

void XJNI_HashMapDelete(XJNI_HashMap* map, const char* key) {
	if (!map || !key) return;
	xjni_mutex_lock(&map->mutex);
	XJNI_HashMapDeleteNoLock(map,key);
	xjni_mutex_unlock(&map->mutex);
}

int XJNI_HashMapReplaceNoLock(XJNI_HashMap* map, const char* key,XJNI_HashValue_t value) {
	if (!map || !key) return 0;
	size_t index = hash(key, map->table_size);
	size_t start = index;

	while (map->hash[index].state != XJNI_SLOT_EMPTY) {
		if (map->hash[index].state == XJNI_SLOT_OCCUPIED && XJNI_strcmp(map->hash[index].key, key) == 0) {
			map->hash[index].value = value;
			return 1;
		}
		index = (index + 1) % map->table_size;
		if (index == start) break;
	}
	return 0;
}

int XJNI_HashMapReplace(XJNI_HashMap* map, const char* key,XJNI_HashValue_t value) {
	if (!map || !key) return 0;
	xjni_mutex_lock(&map->mutex);

	size_t index = hash(key, map->table_size);
	size_t start = index;

	while (map->hash[index].state != XJNI_SLOT_EMPTY) {
		if (map->hash[index].state == XJNI_SLOT_OCCUPIED && XJNI_strcmp(map->hash[index].key, key) == 0) {
			map->hash[index].value = value;
			xjni_mutex_unlock(&map->mutex);
			return 1;
		}
		index = (index + 1) % map->table_size;
		if (index == start) break;
	}

	xjni_mutex_unlock(&map->mutex);
	return 0;
}

int XJNI_HashMapReplaceGetOldNoLock(XJNI_HashMap* map, const char* key,XJNI_HashValue_t newValue,XJNI_HashValue_t* oldValue) {
	if (!map || !key || !oldValue) return 0;
	size_t index = hash(key, map->table_size);
	size_t start = index;

	while (map->hash[index].state != XJNI_SLOT_EMPTY) {
		if (map->hash[index].state == XJNI_SLOT_OCCUPIED && XJNI_strcmp(map->hash[index].key, key) == 0) {
			*oldValue = map->hash[index].value;
			map->hash[index].value = newValue;
			return 1;
		}
		index = (index + 1) % map->table_size;
		if (index == start) break;
	}
	return 0;
}

int XJNI_HashMapReplaceGetOld(XJNI_HashMap* map, const char* key,XJNI_HashValue_t newValue,XJNI_HashValue_t* oldValue) {
	if (!map || !key || !oldValue) return 0;
	xjni_mutex_lock(&map->mutex);

	size_t index = hash(key, map->table_size);
	size_t start = index;

	while (map->hash[index].state != XJNI_SLOT_EMPTY) {
		if (map->hash[index].state == XJNI_SLOT_OCCUPIED && XJNI_strcmp(map->hash[index].key, key) == 0) {
			*oldValue = map->hash[index].value;
			map->hash[index].value = newValue;
			xjni_mutex_unlock(&map->mutex);
			return 1;
		}
		index = (index + 1) % map->table_size;
		if (index == start) break;
	}

	xjni_mutex_unlock(&map->mutex);
	return 0;
}

void XJNI_HashMapAddNoLock(XJNI_HashMap *map, const char *key, XJNI_HashValue_t delta) {
	if (!map || !key) return;
	float load = (float)(map->size + map->tombstones) / map->table_size;
	float dead = (float)map->tombstones / map->table_size;
	if (load >= MAX_LOAD_FACTOR || dead >= TOMBSTONE_CLEAN_RATIO)
		XJNI_HashMapRehash(map, map->table_size * RESIZE_GROW_FACTOR);
	size_t index = hash(key, map->table_size);
	size_t firstTombstone = (size_t)-1;
	while (map->hash[index].state != XJNI_SLOT_EMPTY) {
		if (map->hash[index].state == XJNI_SLOT_OCCUPIED && XJNI_strcmp(map->hash[index].key, key) == 0) {
			map->hash[index].value += delta;
			return;
		}
		if (map->hash[index].state == XJNI_SLOT_TOMBSTONE && firstTombstone == (size_t)-1) {
			firstTombstone = index;
		}
		index = (index + 1) % map->table_size;
	}
	size_t target = (firstTombstone != (size_t)-1) ? firstTombstone : index;
	if (map->hash[target].state == XJNI_SLOT_TOMBSTONE)
		map->tombstones--;
	XJNI_strncpy(map->hash[target].key,key,sizeof(map->hash[target].key));
	map->hash[target].value = delta;
	map->hash[target].state = XJNI_SLOT_OCCUPIED;
	map->size++;
}

void XJNI_HashMapAdd(XJNI_HashMap *map, const char *key, XJNI_HashValue_t delta) {
	if (!map || !key)
		return;
	xjni_mutex_lock(&map->mutex);
	float load = (float)(map->size + map->tombstones) / map->table_size;
	float dead = (float)map->tombstones / map->table_size;
	if (load >= MAX_LOAD_FACTOR || dead >= TOMBSTONE_CLEAN_RATIO)
		XJNI_HashMapRehash(map, map->table_size * RESIZE_GROW_FACTOR);
	size_t index = hash(key, map->table_size);
	size_t firstTombstone = (size_t)-1;
	while (map->hash[index].state != XJNI_SLOT_EMPTY) {
		if (map->hash[index].state == XJNI_SLOT_OCCUPIED &&
				XJNI_strcmp(map->hash[index].key, key) == 0) {
			map->hash[index].value += delta;
			xjni_mutex_unlock(&map->mutex);
			return;
		}
		if (map->hash[index].state == XJNI_SLOT_TOMBSTONE && firstTombstone == (size_t)-1) {
			firstTombstone = index;
		}
		index = (index + 1) % map->table_size;
	}
	size_t target = (firstTombstone != (size_t)-1) ? firstTombstone : index;
	if (map->hash[target].state == XJNI_SLOT_TOMBSTONE)
		map->tombstones--;
	XJNI_strncpy(map->hash[target].key,key,sizeof(map->hash[target].key));
	map->hash[target].value = delta;
	map->hash[target].state = XJNI_SLOT_OCCUPIED;
	map->size++;
	xjni_mutex_unlock(&map->mutex);
}

int XJNI_HashMapCompareReplaceNoLock(XJNI_HashMap* map, const char* key, XJNI_HashValue_t expected, XJNI_HashValue_t desired) {
	if (!map || !key) return 0;
	size_t index = hash(key, map->table_size);
	size_t start = index;
	while (map->hash[index].state != XJNI_SLOT_EMPTY) {
		if (map->hash[index].state == XJNI_SLOT_OCCUPIED && XJNI_strcmp(map->hash[index].key, key) == 0) {
			if (map->hash[index].value == expected) {
				map->hash[index].value = desired;
				return 1;
			}
			break;
		}
		index = (index + 1) % map->table_size;
		if (index == start) break;
	}
	return 0;
}

int XJNI_HashMapCompareReplace(XJNI_HashMap* map, const char* key, XJNI_HashValue_t expected, XJNI_HashValue_t desired) {
	if (!map || !key) return 0;
	xjni_mutex_lock(&map->mutex);

	size_t index = hash(key, map->table_size);
	size_t start = index;

	while (map->hash[index].state != XJNI_SLOT_EMPTY) {
		if (map->hash[index].state == XJNI_SLOT_OCCUPIED && XJNI_strcmp(map->hash[index].key, key) == 0) {
			if (map->hash[index].value == expected) {
				map->hash[index].value = desired;
				xjni_mutex_unlock(&map->mutex);
				return 1;
			}
			break;
		}
		index = (index + 1) % map->table_size;
		if (index == start) break;
	}

	xjni_mutex_unlock(&map->mutex);
	return 0;
}

int XJNI_HashMapReplaceOrInsertNoLock(XJNI_HashMap* map, const char* key,XJNI_HashValue_t value) {
	if (!map || !key) return 0;

	float load = (float)(map->size + map->tombstones) / map->table_size;
	float dead = (float)map->tombstones / map->table_size;

	if (load >= MAX_LOAD_FACTOR || dead >= TOMBSTONE_CLEAN_RATIO)
		XJNI_HashMapRehash(map, map->table_size * RESIZE_GROW_FACTOR);

	size_t index = hash(key, map->table_size);
	size_t firstTombstone = (size_t)-1;

	while (map->hash[index].state != XJNI_SLOT_EMPTY) {
		if (map->hash[index].state == XJNI_SLOT_OCCUPIED && XJNI_strcmp(map->hash[index].key, key) == 0) {
			map->hash[index].value = value;
			return 1;
		}
		if (map->hash[index].state == XJNI_SLOT_TOMBSTONE && firstTombstone == (size_t)-1)
			firstTombstone = index;

		index = (index + 1) % map->table_size;
	}

	size_t target = (firstTombstone != (size_t)-1) ? firstTombstone : index;
	if (map->hash[target].state == XJNI_SLOT_TOMBSTONE) map->tombstones--;

	XJNI_strncpy(map->hash[target].key, key, sizeof(map->hash[target].key));
	map->hash[target].value = value;
	map->hash[target].state = XJNI_SLOT_OCCUPIED;
	map->size++;
	return 0;
}

int XJNI_HashMapReplaceOrInsert(XJNI_HashMap* map, const char* key,XJNI_HashValue_t value) {
	if (!map || !key) return 0;
	xjni_mutex_lock(&map->mutex);

	float load = (float)(map->size + map->tombstones) / map->table_size;
	float dead = (float)map->tombstones / map->table_size;

	if (load >= MAX_LOAD_FACTOR || dead >= TOMBSTONE_CLEAN_RATIO)
		XJNI_HashMapRehash(map, map->table_size * RESIZE_GROW_FACTOR);

	size_t index = hash(key, map->table_size);
	size_t firstTombstone = (size_t)-1;

	while (map->hash[index].state != XJNI_SLOT_EMPTY) {
		if (map->hash[index].state == XJNI_SLOT_OCCUPIED && XJNI_strcmp(map->hash[index].key, key) == 0) {
			map->hash[index].value = value;
			xjni_mutex_unlock(&map->mutex);
			return 1;
		}
		if (map->hash[index].state == XJNI_SLOT_TOMBSTONE && firstTombstone == (size_t)-1)
			firstTombstone = index;

		index = (index + 1) % map->table_size;
	}

	size_t target = (firstTombstone != (size_t)-1) ? firstTombstone : index;
	if (map->hash[target].state == XJNI_SLOT_TOMBSTONE) map->tombstones--;

	XJNI_strncpy(map->hash[target].key, key, sizeof(map->hash[target].key));
	map->hash[target].value = value;
	map->hash[target].state = XJNI_SLOT_OCCUPIED;
	map->size++;

	xjni_mutex_unlock(&map->mutex);
	return 0;
}

void XJNI_HashMapClearNoLock(XJNI_HashMap* map) {
	if (!map) return;
	for (size_t i = 0; i < map->table_size; i++) {
		if (map->hash[i].state == XJNI_SLOT_OCCUPIED) map->hash[i].state = XJNI_SLOT_EMPTY;
	}
	map->size = 0;
	map->tombstones = 0;
}

void XJNI_HashMapClear(XJNI_HashMap* map) {
	if (!map) return;
	xjni_mutex_lock(&map->mutex);
	XJNI_HashMapClearNoLock(map);
	xjni_mutex_unlock(&map->mutex);
}

int XJNI_HashMapCheckInvariantNoLock(XJNI_HashMap* map) {
	if (!map) return 0;
	size_t occ = 0, tomb = 0;
	for (size_t i = 0; i < map->table_size; i++) {
		if (map->hash[i].state == XJNI_SLOT_OCCUPIED) occ++;
		if (map->hash[i].state == XJNI_SLOT_TOMBSTONE) tomb++;
	}
	return (occ == map->size) && (tomb == map->tombstones) && (occ + tomb <= map->table_size);
}

int XJNI_HashMapCheckInvariant(XJNI_HashMap* map) {
	if (!map) return 0;
	xjni_mutex_lock(&map->mutex);
	int ret = XJNI_HashMapCheckInvariantNoLock(map);
	xjni_mutex_unlock(&map->mutex);
	return ret;
}

void XJNI_HashMapFreeNoLock(XJNI_HashMap* map) {
	if (!map) return;
	free(map->hash);
	jmemset(map, 0, sizeof(*map));
}

void XJNI_HashMapFree(XJNI_HashMap* map) {
	if (!map) return;
	xjni_mutex_lock(&map->mutex);
	free(map->hash);
	xjni_mutex_unlock(&map->mutex);
	xjni_mutex_destroy(&map->mutex);
	jmemset(map, 0, sizeof(*map));
}
