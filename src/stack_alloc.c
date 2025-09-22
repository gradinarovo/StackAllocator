/**
 * @file        stack_alloc.c
 * @brief       Stack-based memory allocator implementation
 * @details     This module provides a simple, fast, and deterministic memory allocator
 *              that operates on a pre-allocated buffer in a LIFO manner.
 */

/* ================================ Includes ================================ */
#include "stack_alloc.h"
#include "stack_alloc_cfg.h"
#include "helper_routines.h"

/**
 * @brief       Aligns an address to the specified alignment boundary
 * @param[in]   address   The address to be aligned
 * @param[in]   alignment The alignment boundary (must be a power of 2)
 * @return      The aligned address
 * @note        This is an internal helper function not meant to be called directly
 */
static inline uint32 AlignUp(uint32 address, uint32 alignment) 
{
    /* Calculate how many bytes we need to add to reach alignment */
    uint32 remainder = address % alignment;

    /* If already aligned, return as is */
    if (remainder == 0U)
    {
        return address;
    }

    /* Add required bytes to reach next alignment boundary */
    return address + (alignment - remainder);
}

/**
 * @brief       Gets the aligned start address of the stack buffer
 * @param[in]   sa Pointer to the stack allocator instance
 * @return      Aligned start address of the buffer
 * @note        This is an internal helper function not meant to be called directly
 */
static inline uint8* GetAlignedStart(const TStack_alloc* sa)
{
    /* Ensure the buffer start is properly aligned */
    return (uint8*)AlignUp((uint32)sa->buffer_start, STACK_ALLOC_ALIGNMENT);
}

/**
 * @brief       Initializes the stack allocator with a pre-allocated buffer
 * @param[in]   sa           Pointer to the stack allocator instance to initialize
 * @param[in]   buffer       Pointer to the pre-allocated memory buffer
 * @param[in]   buffer_size  Size of the provided buffer in bytes
 * @return      STACK_ALLOC_OK on success, error code otherwise
 * @retval      STACK_ALLOC_ERROR_INVALID_PARAM  If any parameter is invalid
 * @retval      STACK_ALLOC_ERROR_OUT_OF_MEMORY  If buffer is too small after alignment
 * @note        The buffer must be large enough to accommodate the required alignment
 */
TStack_alloc_error StackAlloc_Init(TStack_alloc* sa, void* buffer, uint32 buffer_size)
{
    /* Check for NULL pointers and minimum buffer size */
    if ((sa == NULL_PTR) || (buffer == NULL_PTR) || (buffer_size < STACK_ALLOC_ALIGNMENT)) 
    {
        return STACK_ALLOC_ERROR_INVALID_PARAM;
    }

    /* Initialize buffer boundaries and capacity */
    sa->buffer_start = (uint8*)buffer;
    sa->buffer_end   = sa->buffer_start + buffer_size;
    sa->capacity     = (uint32)buffer_size;
    
    /* Align the current pointer to the required boundary */
    sa->current = GetAlignedStart(sa);

    /* Verify we have at least some space after alignment */
    if (sa->current >= sa->buffer_end)
    {
        return STACK_ALLOC_ERROR_OUT_OF_MEMORY;
    }

    return STACK_ALLOC_OK;
}

/**
 * @brief       Allocates a block of memory from the stack
 * @param[in]   sa    Pointer to the stack allocator instance
 * @param[in]   size  Number of bytes to allocate
 * @return      Pointer to the allocated memory, or NULL_PTR on failure
 * @note        The returned pointer is guaranteed to be aligned to STACK_ALLOC_ALIGNMENT
 * @note        This function is not thread-safe
 */
void* StackAlloc_Alloc(TStack_alloc* sa, uint32 size)
{
    /* Check for invalid parameters */
    if ((sa == NULL_PTR) || (size == 0U))
    {
        return NULL_PTR;
    }

    /* Align the current pointer and calculate new top */
    uint8* aligned_ptr = (uint8*)AlignUp((uint32)sa->current, STACK_ALLOC_ALIGNMENT);
    uint8* new_top = aligned_ptr + size;

    /* Check for overflow or out of memory */
    if ((new_top < aligned_ptr) || (new_top > sa->buffer_end))
    {
        return NULL_PTR;
    }

    /* Update current pointer and return allocated block */
    sa->current = new_top;
    return aligned_ptr;
}

/**
 * @brief       Allocates and zero-initializes a block of memory
 * @param[in]   sa    Pointer to the stack allocator instance
 * @param[in]   num   Number of elements to allocate
 * @param[in]   size  Size of each element in bytes
 * @return      Pointer to the allocated and zeroed memory, or NULL_PTR on failure
 * @note        This is equivalent to calloc() but uses the stack allocator
 */
void* StackAlloc_Calloc(TStack_alloc* sa, uint32 num, uint32 size)
{
    /* Check for zero values */
    if ((num == 0U) || (size == 0U))
    {
        return NULL_PTR;
    }

    /* Check for multiplication overflow */
    if (num > (SIZE_MAX / size))
    {
        return NULL_PTR;
    }

    /* Calculate total size needed */
    uint32 total = (uint32)(num * size);

    /* Allocate memory */
    void* ptr = StackAlloc_Alloc(sa, total);

    /* Zero-initialize the allocated memory */
    if (ptr != NULL_PTR)
    {
        (void)mem_set(ptr, 0U, total);
    }

    return ptr;
}

/**
 * @brief       Frees memory back to a previously saved marker
 * @param[in]   sa      Pointer to the stack allocator instance
 * @param[in]   marker  Marker to free back to (must be obtained from StackAlloc_GetMarker)
 * @return      STACK_ALLOC_OK on success, error code otherwise
 * @retval      STACK_ALLOC_ERROR_INVALID_PARAM    If any parameter is invalid
 * @retval      STACK_ALLOC_ERROR_INVALID_MARKER   If marker is outside buffer or not aligned
 * @retval      STACK_ALLOC_ERROR_NOT_LIFO         If trying to free in non-LIFO order
 * @note        This function can only free memory in LIFO order
 */
TStack_alloc_error StackAlloc_FreeToMarker(TStack_alloc* sa, void* marker)
{
    /* Check for NULL pointers */
    if ((sa == NULL_PTR) || (marker == NULL_PTR))
    {
        return STACK_ALLOC_ERROR_INVALID_PARAM;
    }

    /* Cast marker to byte pointer */
    uint8* mark = (uint8*)marker;

    /* Validate marker is within buffer bounds and properly aligned */
    if (mark < sa->buffer_start || mark >= sa->current)
    {
        return STACK_ALLOC_ERROR_INVALID_MARKER;
    }

    // Check if marker is properly aligned
    if ((mark != NULL_PTR) &&
        (((uint32)mark & (STACK_ALLOC_ALIGNMENT - 1U)) != 0U))
    {
        return STACK_ALLOC_ERROR_INVALID_MARKER;
    }

    // Check LIFO order - This check is not needed in the free function
    // as we're using the marker directly

    /* Update current pointer to free memory */
    sa->current = mark;

    return STACK_ALLOC_OK;
}

/**
 * @brief       Resets the stack allocator to its initial state
 * @param[in]   sa  Pointer to the stack allocator instance
 * @note        This function is safe to call with a NULL pointer
 * @note        After reset, all previously allocated memory is considered freed
 */
void StackAlloc_Reset(TStack_alloc* sa)
{
    if (sa != NULL_PTR)
    {
        /* Reset current pointer to the aligned start of the buffer */
        sa->current = GetAlignedStart(sa);
    }
}

/**
 * @brief       Gets the total capacity of the stack allocator
 * @param[in]   sa  Pointer to the stack allocator instance
 * @return      Total capacity in bytes, or 0 if sa is NULL_PTR
 */
uint32 StackAlloc_GetCapacity(const TStack_alloc* sa)
{
    return (sa != NULL_PTR) ? sa->capacity : 0U;
}

/**
 * @brief       Gets the amount of memory currently in use in bytes
 * @param[in]   sa  Pointer to the stack allocator instance
 * @return      Number of bytes currently allocated, or 0 if sa is NULL_PTR
 * @note        The returned value includes any internal alignment padding
 */
uint32 StackAlloc_GetUsed(const TStack_alloc* sa)
{
    if (sa == NULL_PTR)
    {
        return 0U;
    }
    
    /* Calculate used space as difference between current and aligned start */
    uint8* aligned_start = GetAlignedStart(sa);
    return (uint32)(sa->current - aligned_start);
}

/**
 * @brief       Gets the amount of memory available for allocation
 * @param[in]   sa  Pointer to the stack allocator instance
 * @return      Number of bytes available for allocation, or 0 if sa is NULL_PTR
 * @note        The actual available space might be less due to alignment requirements
 */
uint32 StackAlloc_GetAvailable(const TStack_alloc* sa)
{
    return (sa != NULL_PTR) ? (uint32)(sa->buffer_end - sa->current) : 0U;
}

/**
 * @brief       Validates the internal state of the stack allocator
 * @param[in]   sa  Pointer to the stack allocator instance to validate
 * @return      STACK_ALLOC_OK if valid, STACK_ALLOC_ERROR_CORRUPTED_STATE otherwise
 * @note        This function performs sanity checks on the allocator's internal state
 */
TStack_alloc_error StackAlloc_Validate(const TStack_alloc* sa)
{
    /* Check for NULL pointers in the allocator structure */
    if ((sa == NULL_PTR) || 
        (sa->buffer_start == NULL_PTR) || 
        (sa->buffer_end == NULL_PTR))
    {
        return STACK_ALLOC_ERROR_CORRUPTED_STATE;
    }
    
    /* Verify buffer boundaries are valid */
    if (sa->buffer_start > sa->buffer_end)
    {
        return STACK_ALLOC_ERROR_CORRUPTED_STATE;
    }
    
    /* Verify current pointer is within valid range */
    uint8* aligned_start = GetAlignedStart(sa);
    if ((sa->current < aligned_start) || (sa->current > sa->buffer_end))
    {
        return STACK_ALLOC_ERROR_CORRUPTED_STATE;
    }
    
    return STACK_ALLOC_OK;
}