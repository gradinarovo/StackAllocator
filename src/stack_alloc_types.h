/**
 * @file       stack_alloc_types.h
 * @brief      Stack Allocator Type Definitions
 * @details    Type definitions and error codes for the Stack Allocator module
 */

#ifndef STACK_ALLOC_TYPES_H
#define STACK_ALLOC_TYPES_H

#include "std_types.h"  /* For standard types */

/* Define SIZE_MAX if not already defined */
#ifndef SIZE_MAX
    #define SIZE_MAX ((uint32)-1)  /* Maximum value for uint32 */
#endif

/**
 * @brief Stack Allocator error codes
 */
typedef uint8 TStack_alloc_error;

/**
 * @brief Stack Allocator error code values
 */
#define STACK_ALLOC_OK                      (0x00u)  /**< Operation completed successfully */
#define STACK_ALLOC_ERROR_INVALID_PARAM     (0x01u)  /**< Invalid parameter passed to function */
#define STACK_ALLOC_ERROR_OUT_OF_MEMORY     (0x02u)  /**< Not enough memory available for allocation */
#define STACK_ALLOC_ERROR_CORRUPTED_STATE   (0x03u)  /**< Memory corruption detected */
#define STACK_ALLOC_ERROR_INVALID_MARKER    (0x04u)  /**< Invalid marker or stack position */
#define STACK_ALLOC_ERROR_NOT_LIFO          (0x05u)  /**< Free operation violates LIFO order */

/**
 * @brief Internal structure for the stack allocator
 */
typedef struct {
    uint8* buffer_start;  /**< Start of the entire memory region */
    uint8* buffer_end;    /**< End of the entire memory region (one past the last byte) */
    uint8* current;       /**< Pointer to the current top of the stack */
    uint32 capacity;      /**< Total size of the buffer in bytes */
} TStack_alloc;

#endif /* STACK_ALLOC_TYPES_H */