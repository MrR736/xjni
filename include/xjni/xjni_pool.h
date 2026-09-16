/**
 * @file xjni_pool.h
 * @brief Memory Pool Utility (Thread-safe / Debug-hardened)
 *
 * This module provides a fixed-size memory pool for efficient allocation
 * and deallocation of elements. Optionally thread-safe using mutexes.
 * Debug mode supports memory poisoning and double-free detection.
 *
 * @author MrR736
 * @date 2025
 * @copyright MIT
 */

#ifndef __XJNI_POOL_H__
#define __XJNI_POOL_H__

#include <stddef.h>
#include "xjnidef.h"
#include "xjni_thread.h"

/**
 * @struct xjni_pool
 * @brief Memory pool structure.
 *
 * @var pool        Pointer to the contiguous memory block.
 * @var free_list   LIFO linked list of free elements.
 * @var capacity    Total number of elements in the pool.
 * @var size        Number of elements currently allocated.
 * @var element_size Size of each element in bytes (aligned to void*).
 * @var mutex       Optional mutex for thread safety.
 * @var UseMutex    Non-zero if mutex is used.
 */
typedef struct xjni_pool {
	void* pool;
	void* free_list;
	size_t capacity;
	size_t size;
	size_t element_size;
	xjni_mutex_t mutex;
	unsigned char UseMutex;
} xjni_pool;

/**
 * @enum xjni_pool_result
 * @brief Return codes for pool operations.
 */
typedef enum {
	XJNI_POOL_OK = 0,          /**< Operation successful */
	XJNI_POOL_ERROR_IN_USE,    /**< Pool has allocated elements */
	XJNI_POOL_ERROR_NULL       /**< Pool pointer is NULL */
} xjni_pool_result;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a memory pool.
 *
 * @param capacity      Number of elements in the pool.
 * @param element_size  Size of each element (in bytes). Will be aligned to void*.
 * @param use_mutex     Non-zero to make pool thread-safe.
 *
 * @return Pointer to the created xjni_pool, or NULL on allocation failure.
 */
XJNI_API xjni_pool* xjni_pool_Create(size_t capacity, size_t element_size, int use_mutex);

/**
 * @brief Allocate an element from the pool.
 *
 * @param mp Pointer to the memory pool.
 *
 * @return Pointer to an element, or NULL if the pool is exhausted.
 */
XJNI_API void* xjni_pool_AllocFromPool(xjni_pool* mp);

/**
 * @brief Free a previously allocated element back to the pool.
 *
 * Performs debug checks in DEBUG builds:
 * - Validates pointer is within pool bounds
 * - Checks alignment
 * - Detects double-free
 * - Poisons memory after free
 *
 * @param mp   Pointer to the memory pool.
 * @param elem Pointer to the element to free.
 */
XJNI_API void xjni_pool_FreeElement(xjni_pool* mp, void* elem);

/**
 * @brief Free the entire memory pool.
 *
 * @param mp Pointer to the memory pool.
 *
 * @return XJNI_POOL_OK if pool successfully freed,
 *         XJNI_POOL_ERROR_IN_USE if elements are still allocated,
 *         XJNI_POOL_ERROR_NULL if mp is NULL.
 */
XJNI_API int xjni_pool_Free(xjni_pool* mp);

#ifdef DEBUG
/**
 * @brief Get number of currently allocated elements.
 *
 * @param mp Pointer to the memory pool.
 * @return Number of used elements, or 0 if mp is NULL.
 */
XJNI_API size_t xjni_pool_UsedCount(xjni_pool* mp);

/**
 * @brief Get number of free elements in the pool.
 *
 * @param mp Pointer to the memory pool.
 * @return Number of free elements, or 0 if mp is NULL.
 */
XJNI_API size_t xjni_pool_FreeCount(xjni_pool* mp);

/**
 * @brief Get the total capacity of the pool.
 *
 * @param mp Pointer to the memory pool.
 * @return Total number of elements in the pool, or 0 if mp is NULL.
 */
XJNI_API size_t xjni_pool_Capacity(xjni_pool* mp);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __XJNI_POOL_H__ */
