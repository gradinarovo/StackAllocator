/**
 * @file        helper_routines.c
 * @brief       Implementation of helper utility functions
 * @details     This file contains implementations of various helper functions
 *              that provide common functionality used throughout the application.
 */

#include "helper_routines.h"

/**
 * @brief Fill a block of memory with a specified value
 * 
 * @param dest Pointer to the block of memory to fill
 * @param value Value to be set (converted to an unsigned char)
 * @param num Number of bytes to be set to the value
 * @return void* A pointer to the memory area dest, or NULL if dest is NULL
 * 
 * @note This implementation is optimized for performance while maintaining
 *       portability and safety with NULL pointer checks.
 */
void* mem_set(void* dest, sint32 value, uint32 num) 
{
    uint8* ptr = (uint8*)dest;
    uint8 byte_value = (uint8)value;
    
    if (dest == NULL_PTR) 
    {
        return NULL_PTR;
    }
    
    while (num-- > 0) 
    {
        *ptr++ = byte_value;
    }
    
    return dest;
}