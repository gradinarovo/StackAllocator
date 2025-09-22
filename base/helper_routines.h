/**
 * @file        helper_routines.h
 * @brief       Helper utility functions
 * @details     This file contains declarations of various helper functions
 *              that provide common functionality used throughout the application.
 */

#ifndef HELPER_ROUTINES_H
#define HELPER_ROUTINES_H

#include "std_types.h"  /* For standard types */

/**
 * @brief Fill a block of memory with a specified value
 * 
 * @param dest Pointer to the block of memory to fill
 * @param value Value to be set (converted to an unsigned char)
 * @param num Number of bytes to be set to the value
 * @return void* A pointer to the memory area dest
 * 
 * @note This is a custom implementation of the standard memset function.
 *       It handles NULL pointer checks and returns NULL if dest is NULL.
 */
void* mem_set(void* dest, sint32 value, uint32 num);

#endif /* HELPER_ROUTINES_H */
