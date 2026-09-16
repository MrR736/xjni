/**
 * @file xjni_hashmap.h
 * @brief Thread-safe open-addressing hash map utility.
 *
 * Provides a linear-probing hash map implementation with tombstone-based
 * deletion and optional thread-safety via internal mutex protection.
 *
 * Designed for lightweight native runtime usage (e.g., JNI infrastructure),
 * but does not depend on JNI semantics.
 *
 * Features:
 *  - Open addressing with linear probing
 *  - Tombstone-based deletion
 *  - Automatic resizing and tombstone cleanup
 *  - Optional thread-safe API
 *  - Atomic-style replace and compare-and-swap operations
 *
 * @author MrR736
 * @date 2026
 * @license MIT
 */

#ifndef XJNI_HASHMAP_H
#define XJNI_HASHMAP_H

#include <inttypes.h>

#include "xjni_thread.h"
#include "xjnidef.h"

/**
 * @defgroup XJNI_HashMap HashMap
 * @brief Thread-safe open-addressing hash map implementation.
 *
 * Provides a lightweight, tombstone-based linear probing hash map
 * with optional internal synchronization.
 *
 * @{
 */

/**
 * @def XJNI_HASHMAP_KEY_SIZE
 * @brief Maximum allowed key length (including null terminator).
 *
 * @warning Keys longer than this value must be rejected by implementation.
 */
#define XJNI_HASHMAP_KEY_SIZE 128

/**
 * @def XJNI_HashMapAppend
 * @brief Convenience macro to insert key with delta = 1.
 */
#define XJNI_HashMapAppend(map,key) XJNI_HashMapAdd((map),(key),1)

/**
 * @def XJNI_HashMapIncrement
 * @brief Convenience macro to increment key value by 1.
 */
#define XJNI_HashMapIncrement(map,key) XJNI_HashMapAdd((map),(key),1)

/**
 * @def XJNI_HashMapAppendNoLock
 * @brief Convenience macro to insert key with delta = 1.
 */
#define XJNI_HashMapAppendNoLock(map,key) XJNI_HashMapAddNoLock((map),(key),1)

/**
 * @def XJNI_HashMapIncrementNoLock
 * @brief Convenience macro to increment key value by 1.
 */
#define XJNI_HashMapIncrementNoLock(map,key) XJNI_HashMapAddNoLock((map),(key),1)

/**
 * @enum XJNI_HashSlotState
 * @brief Internal slot state for linear probing.
 */
typedef enum {
	XJNI_SLOT_EMPTY = 0,	  /**< Slot has never been used */
	XJNI_SLOT_OCCUPIED,	   /**< Slot contains a valid key-value pair */
	XJNI_SLOT_TOMBSTONE	   /**< Slot was deleted and marked reusable */
} XJNI_HashSlotState;

#ifdef __XJNI_x64__
typedef uint64_t XJNI_HashValue_t;
#define XJNI_HASH_FMT PRIu64
#else
typedef uint32_t XJNI_HashValue_t;
#define XJNI_HASH_FMT PRIu32
#endif

#define XJNI_HASHMAP_NOT_FOUND ((XJNI_HashValue_t)(-1))

/**
 * @struct XJNI_HashMapKeyValue
 * @brief Single key-value entry stored in the hash table.
 */
typedef struct {
	char key[XJNI_HASHMAP_KEY_SIZE];	/**< Null-terminated key string */
	XJNI_HashValue_t value;				/**< Associated integer value */
	unsigned char state;			/**< Slot state */
} XJNI_HashMapKeyValue;

/**
 * @struct XJNI_HashMap
 * @brief Hash map container structure.
 *
 * Thread-safe variants of API functions acquire the internal mutex.
 * NoLock variants must only be used when external synchronization is guaranteed.
 */
typedef struct {
	size_t table_size;		/**< Total number of slots */
	size_t size;			/**< Number of occupied slots */
	size_t tombstones;		/**< Number of tombstone slots */
	XJNI_HashMapKeyValue* hash;	/**< Pointer to slot array */
	xjni_mutex_t mutex;		/**< xjni mutex */
} XJNI_HashMap;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize a hash map without mutex.
 *
 * @param map Pointer to hash map instance.
 * @param capacity Initial number of slots (will be adjusted internally if needed).
 *
 * @return 1 on success, 0 on allocation failure.
 *
 * @note Must be called before any other operation.
 */
int XJNI_HashMapInitNoMutex(XJNI_HashMap* map, size_t capacity);

/**
 * @brief Initialize a hash map.
 *
 * @param map Pointer to hash map instance.
 * @param capacity Initial number of slots (will be adjusted internally if needed).
 *
 * @return 1 on success, 0 on allocation failure.
 *
 * @note Must be called before any other operation.
 */
int XJNI_HashMapInit(XJNI_HashMap* map, size_t capacity);

/**
 * @brief Free all memory associated with the hash map without lock.
 *
 * Destroys internal mutex and releases slot array.
 *
 * @param map Pointer to hash map instance.
 */
void XJNI_HashMapFreeNoLock(XJNI_HashMap* map);

/**
 * @brief Free all memory associated with the hash map.
 *
 * Destroys internal mutex and releases slot array.
 *
 * @param map Pointer to hash map instance.
 */
void XJNI_HashMapFree(XJNI_HashMap* map);

/**
 * @brief Clear all entries without freeing memory without locking.
 *
 * Resets size and tombstone count but keeps allocated capacity.
 *
 * @param map Pointer to hash map instance.
 */
void XJNI_HashMapClearNoLock(XJNI_HashMap* map);

/**
 * @brief Clear all entries without freeing memory.
 *
 * Resets size and tombstone count but keeps allocated capacity.
 *
 * @param map Pointer to hash map instance.
 */
void XJNI_HashMapClear(XJNI_HashMap* map);

/**
 * @brief Insert or update without locking.
 *
 * Caller must ensure external synchronization.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 * @param value Value to insert.
 */
void XJNI_HashMapInsertNoLock(XJNI_HashMap* map, const char* key, XJNI_HashValue_t value);

/**
 * @brief Insert or update a key-value pair (thread-safe).
 *
 * Automatically handles tombstone reuse and resizing.
 *
 * @param map Pointer to hash map.
 * @param key Null-terminated key string.
 * @param value Value to insert.
 */
void XJNI_HashMapInsert(XJNI_HashMap* map, const char* key, XJNI_HashValue_t value);

/**
 * @brief Replace value of an existing key without locking.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 * @param value New value.
 *
 * @return 1 if replaced, 0 if key not found.
 */
int XJNI_HashMapReplaceNoLock(XJNI_HashMap* map, const char* key, XJNI_HashValue_t value);

/**
 * @brief Replace value of an existing key.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 * @param value New value.
 *
 * @return 1 if replaced, 0 if key not found.
 */
int XJNI_HashMapReplace(XJNI_HashMap* map, const char* key, XJNI_HashValue_t value);

/**
 * @brief Replace value and retrieve old value without locking.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 * @param newValue Value to assign.
 * @param oldValue Output parameter for previous value (if key exists).
 *
 * @return 1 if replaced, 0 if key not found.
 */
int XJNI_HashMapReplaceGetOldNoLock(XJNI_HashMap* map,const char* key,XJNI_HashValue_t newValue,XJNI_HashValue_t* oldValue);

/**
 * @brief Replace value and retrieve old value.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 * @param newValue Value to assign.
 * @param oldValue Output parameter for previous value (if key exists).
 *
 * @return 1 if replaced, 0 if key not found.
 */
int XJNI_HashMapReplaceGetOld(XJNI_HashMap* map,const char* key,XJNI_HashValue_t newValue,XJNI_HashValue_t* oldValue);

/**
 * @brief Compare-and-replace operation without locking.
 *
 * Replaces value only if current value matches expected.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 * @param expected Expected current value.
 * @param desired New value if comparison succeeds.
 *
 * @return 1 if replaced, 0 otherwise.
 */
int XJNI_HashMapCompareReplaceNoLock(XJNI_HashMap* map,const char* key,XJNI_HashValue_t expected,XJNI_HashValue_t desired);

/**
 * @brief Compare-and-replace operation.
 *
 * Replaces value only if current value matches expected.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 * @param expected Expected current value.
 * @param desired New value if comparison succeeds.
 *
 * @return 1 if replaced, 0 otherwise.
 */
int XJNI_HashMapCompareReplace(XJNI_HashMap* map,const char* key,XJNI_HashValue_t expected,XJNI_HashValue_t desired);

/**
 * @brief Replace value if key exists, otherwise insert without locking.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 * @param value Value to assign.
 *
 * @return 1 if replaced, 0 if inserted.
 */
int XJNI_HashMapReplaceOrInsertNoLock(XJNI_HashMap* map,const char* key,XJNI_HashValue_t value);

/**
 * @brief Replace value if key exists, otherwise insert.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 * @param value Value to assign.
 *
 * @return 1 if replaced, 0 if inserted.
 */
int XJNI_HashMapReplaceOrInsert(XJNI_HashMap* map,const char* key,XJNI_HashValue_t value);

/**
 * @brief Add delta to key value without locking.
 *
 * If key does not exist, inserts it with value = delta.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 * @param delta Value increment.
 */
void XJNI_HashMapAddNoLock(XJNI_HashMap* map,const char* key,XJNI_HashValue_t delta);

/**
 * @brief Add delta to key value.
 *
 * If key does not exist, inserts it with value = delta.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 * @param delta Value increment.
 */
void XJNI_HashMapAdd(XJNI_HashMap* map,const char* key,XJNI_HashValue_t delta);

/**
 * @brief Try to retrieve value without locking.
 *
 * Caller must ensure external synchronization.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 * @param out Optional output pointer.
 *
 * @return 1 if found, 0 otherwise.
 */
int XJNI_HashMapTryGetNoLock(XJNI_HashMap* map,const char* key,XJNI_HashValue_t* out);

/**
 * @brief Try to retrieve value associated with key (thread-safe).
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 * @param out Optional output pointer for value.
 *
 * @return 1 if key found, 0 otherwise.
 */
int XJNI_HashMapTryGet(XJNI_HashMap* map,const char* key,XJNI_HashValue_t* out);

/**
 * @brief Search for key and return value without locking.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 *
 * @return Value if found, -1 otherwise.
 */
XJNI_HashValue_t XJNI_HashMapSearchNoLock(XJNI_HashMap* map,const char* key);

/**
 * @brief Search for key and return value.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 *
 * @return Value if found, -1 otherwise.
 */
XJNI_HashValue_t XJNI_HashMapSearch(XJNI_HashMap* map,const char* key);

/**
 * @brief Delete key from hash map without locking.
 *
 * Marks slot as tombstone.
 * May trigger automatic cleanup.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 */
void XJNI_HashMapDeleteNoLock(XJNI_HashMap* map,const char* key);

/**
 * @brief Delete key from hash map.
 *
 * Marks slot as tombstone.
 * May trigger automatic cleanup.
 *
 * @param map Pointer to hash map.
 * @param key Key string.
 */
void XJNI_HashMapDelete(XJNI_HashMap* map,const char* key);

/**
 * @brief Verify internal invariants (debug utility) without locking.
 *
 * Checks consistency of size, tombstones, and slot states.
 *
 * @param map Pointer to hash map.
 *
 * @return 1 if consistent, 0 otherwise.
 */
int XJNI_HashMapCheckInvariantNoLock(XJNI_HashMap* map);

/**
 * @brief Verify internal invariants (debug utility).
 *
 * Checks consistency of size, tombstones, and slot states.
 *
 * @param map Pointer to hash map.
 *
 * @return 1 if consistent, 0 otherwise.
 */
int XJNI_HashMapCheckInvariant(XJNI_HashMap* map);

#ifdef __cplusplus
}
#endif

/** @} */ /* End of XJNI_HashMap group */

#endif /* XJNI_HASHMAP_H */
