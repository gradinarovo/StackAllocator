/**
 * @file        stack_alloc.h
 * @brief       Stack-based memory allocator API
 * @details     Provides a simple, fast, and deterministic memory allocator
 *              that operates on a pre-allocated buffer in a LIFO manner.
 * 
 * @note        This implementation is not thread-safe. If thread safety is required,
 *              external synchronization must be implemented by the caller.
 */

#ifndef STACK_ALLOC_H
#define STACK_ALLOC_H

#include "stack_alloc_types.h"

/**
 * @brief       Initializes a stack allocator with a user-provided buffer
 * @param[in]   sa           Pointer to the stack allocator instance to initialize
 * @param[in]   buffer       Pointer to the memory buffer to use
 * @param[in]   buffer_size  Size of the provided buffer in bytes
 * @return      Error code indicating success or failure
 * @retval      STACK_ALLOC_OK if initialization was successful
 * @retval      STACK_ALLOC_ERROR_NULL_PTR if sa or buffer is NULL
 * @retval      STACK_ALLOC_ERROR_INVALID_SIZE if buffer_size is too small
 * @note        The buffer must be aligned to STACK_ALLOC_ALIGNMENT for optimal performance
 */
TStack_alloc_error StackAlloc_Init(TStack_alloc* sa, void* buffer, uint32 buffer_size);

/**
 * @brief       Allocates a block of memory from the stack
 * @param[in]   sa     Pointer to the stack allocator instance
 * @param[in]   size   Number of bytes to allocate
 * @return      Pointer to the allocated memory, or NULL_PTR on failure
 * @note        The returned pointer is guaranteed to be aligned to STACK_ALLOC_ALIGNMENT
 * @note        This function is not thread-safe
 */
void* StackAlloc_Alloc(TStack_alloc* sa, uint32 size);

/**
 * @brief       Allocates and zero-initializes a block of memory
 * @param[in]   sa     Pointer to the stack allocator instance
 * @param[in]   num    Number of elements to allocate
 * @param[in]   size   Size of each element in bytes
 * @return      Pointer to the allocated and zeroed memory, or NULL_PTR on failure
 * @note        This is equivalent to calloc() but uses the stack allocator
 */
void* StackAlloc_Calloc(TStack_alloc* sa, uint32 num, uint32 size);

/**
 * @brief       Gets a marker representing the current stack position
 * @param[in]   sa  Pointer to the stack allocator instance
 * @return      Marker representing the current stack position, or NULL_PTR if sa is NULL
 * @note        This marker can be used with StackAlloc_Rewind() to free memory back to this point
 */
void* StackAlloc_GetMarker(const TStack_alloc* sa);

/**
 * @brief       Frees memory back to a previously obtained marker
 * @param[in]   sa      Pointer to the stack allocator instance
 * @param[in]   marker  Marker obtained from StackAlloc_GetMarker()
 * @return      Error code indicating success or failure
 * @retval      STACK_ALLOC_OK if free was successful
 * @retval      STACK_ALLOC_ERROR_NULL_PTR if sa is NULL
 * @retval      STACK_ALLOC_ERROR_INVALID_MARKER if marker is invalid
 * @retval      STACK_ALLOC_ERROR_NOT_LIFO if trying to free in non-LIFO order
 * @note        All memory allocated after the marker will be freed
 * @note        This function enforces LIFO (Last-In-First-Out) ordering
 */
TStack_alloc_error StackAlloc_FreeToMarker(TStack_alloc* sa, void* marker);

/**
 * @brief       Resets the stack allocator, freeing all allocated memory
 * @param[in]   sa  Pointer to the stack allocator instance
 * @note        This is equivalent to StackAlloc_Rewind(sa, sa->buffer)
 */
void StackAlloc_Reset(TStack_alloc* sa);

/**
 * @brief       Gets the total capacity of the stack allocator
 * @param[in]   sa  Pointer to the stack allocator instance
 * @return      Total capacity in bytes, or 0 if sa is NULL_PTR
 */
uint32 StackAlloc_GetCapacity(const TStack_alloc* sa);

/**
 * @brief       Gets the amount of memory currently in use in bytes
 * @param[in]   sa  Pointer to the stack allocator instance
 * @return      Number of bytes currently allocated, or 0 if sa is NULL_PTR
 * @note        The returned value includes any internal alignment padding
 */
uint32 StackAlloc_GetUsed(const TStack_alloc* sa);

/**
 * @brief       Gets the amount of free memory available for allocation
 * @param[in]   sa  Pointer to the stack allocator instance
 * @return      Number of bytes available for allocation, or 0 if sa is NULL_PTR
 */
uint32 StackAlloc_GetAvailable(const TStack_alloc* sa);

/**
 * @brief       Validates the internal state of the stack allocator
 * @param[in]   sa  Pointer to the stack allocator instance
 * @return      Error code indicating the validation result
 * @retval      STACK_ALLOC_OK if the allocator is in a valid state
 * @retval      STACK_ALLOC_ERROR_NULL_PTR if sa is NULL
 * @retval      STACK_ALLOC_ERROR_CORRUPTED if the allocator is corrupted
 */
TStack_alloc_error StackAlloc_Validate(const TStack_alloc* sa);

#endif /* STACK_ALLOC_H */