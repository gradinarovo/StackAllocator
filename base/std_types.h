/**
 * @file        std_types.h
 * @brief       Standard type definitions
 * @details     This header file contains standard type definitions for use in C applications.
 *              It provides fixed-width integer types and boolean type for better portability
 *              across different platforms and compilers.
 */

#ifndef STD_TYPES_H
#define STD_TYPES_H

/* Unsigned integer types */

/**
 * @brief   Unsigned 8-bit integer
 * @details This type can store values from 0 to 255.
 */
typedef unsigned char uint8;

/**
 * @brief   Unsigned 16-bit integer
 * @details This type can store values from 0 to 65,535.
 */
typedef unsigned short uint16;

/**
 * @brief   Unsigned 32-bit integer
 * @details This type can store values from 0 to 4,294,967,295.
 */
typedef unsigned int uint32;

/**
 * @brief   Unsigned 64-bit integer
 * @details This type can store values from 0 to 18,446,744,073,709,551,615.
 */
typedef unsigned long long uint64;

/* Signed integer types */

/**
 * @brief   Signed 8-bit integer
 * @details This type can store values from -128 to 127.
 */
typedef signed char sint8;

/**
 * @brief   Signed 16-bit integer
 * @details This type can store values from -32,768 to 32,767.
 */
typedef signed short sint16;

/**
 * @brief   Signed 32-bit integer
 * @details This type can store values from -2,147,483,648 to 2,147,483,647.
 */
typedef signed int sint32;

/**
 * @brief   Signed 64-bit integer
 * @details This type can store values from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807.
 */
typedef signed long long sint64;

/* Floating point types */

/**
 * @brief   Single precision floating point (32-bit)
 * @details This type is used for single precision floating point numbers.
 */
typedef float float32;

/**
 * @brief   Double precision floating point (64-bit)
 * @details This type is used for double precision floating point numbers.
 */
typedef double float64;

/* Boolean type */

/**
 * @brief   Standard boolean type
 * @details This type is used for boolean values.
 */
typedef uint8 boolean;

/**
 * @brief   Standard boolean values
 * @details This defines the standard boolean values.
 */
#ifndef FALSE
#define FALSE   (0u)
#endif

#ifndef TRUE
#define TRUE    (1u)
#endif

/* Standard return type */

/**
 * @brief   Standard return type for functions
 * @details This type can be used as standard return type for functions.
 *          STD_OK indicates that the function call was successful.
 *          STD_NOT_OK indicates that the function call failed.
 */
typedef enum {
    STD_OK = 0,         /**< Function call successful */
    STD_NOT_OK,         /**< Function call failed */
    STD_BUSY,           /**< Resource busy, try again later */
    STD_IDLE,           /**< No operation in progress */
    STD_PENDING         /**< Operation still in progress */
} Std_ReturnType;

/* Standard pointer definitions */

/**
 * @brief   NULL pointer definition
 * @details This defines a null pointer constant.
 */
#ifndef NULL
#define NULL_PTR ((void *)0)
#else
#define NULL_PTR NULL
#endif

#endif /* STD_TYPES_H */
