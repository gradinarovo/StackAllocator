/**
 * @file        main.c
 * @brief       Entry point for stack allocator test suite
 */

 #include "stack_alloc_test.h"
 #include "std_types.h"
 #include <stdio.h>
 
 int main(void)
 {
     printf("Stack Allocator Unit Test Runner\n");
     printf("--------------------------------\n");
 
     boolean all_passed = StackAlloc_RunAllTests();
 
     if (all_passed == TRUE)
     {
         printf("\nSUCCESS: All tests passed!\n");
         return 0;
     }
     else
     {
         printf("\nFAILURE: Some tests failed.\n");
         return 1;
     }
 }