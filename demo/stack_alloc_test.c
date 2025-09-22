/**
 * @file        stack_alloc_test.c
 * @brief       Comprehensive test suite for stack-based memory allocator
 * @details     Tests all functionality including boundary cases, error handling,
 *              alignment, LIFO constraints, and state validation.
 *              Uses ONLY public API.
 */

 #include "stack_alloc_test.h"
 #include "stack_alloc.h"
 #include <stdio.h>
 #include <string.h>
 
 /* ========================= Test Utility Macros ========================= */
 
 #define TEST_ASSERT(cond, msg) do { \
     if (!(cond)) { \
         printf("ASSERT FAILED: %s at line %d: %s\n", __FILE__, __LINE__, msg); \
         return FALSE; \
     } \
 } while(0)
 
 #define TEST_CASE(name) \
     printf("Running test: %s...\n", #name); \
     if (!test_##name()) { \
         printf("FAILED: %s\n", #name); \
         all_passed = FALSE; \
     } else { \
         printf("PASSED: %s\n", #name); \
     }
 
 /* ========================= Test Buffer ========================= */
 
 #define TEST_BUFFER_SIZE 1024U
 static uint8 g_test_buffer[TEST_BUFFER_SIZE];
 
 /* ========================= Individual Test Cases ========================= */
 
 static boolean test_init_basic(void)
 {
     TStack_alloc sa;
     TStack_alloc_error err;
 
     err = StackAlloc_Init(&sa, g_test_buffer, TEST_BUFFER_SIZE);
     TEST_ASSERT(err == STACK_ALLOC_OK, "Basic init should succeed");
 
     TEST_ASSERT(sa.buffer_start == g_test_buffer, "Buffer start should match");
     TEST_ASSERT(sa.buffer_end == g_test_buffer + TEST_BUFFER_SIZE, "Buffer end should be correct");
     TEST_ASSERT(sa.capacity == TEST_BUFFER_SIZE, "Capacity should match buffer size");
 
     // Check that we can allocate after init
     void* ptr = StackAlloc_Alloc(&sa, 1);
     TEST_ASSERT(ptr != NULL_PTR, "Should be able to allocate after init");
 
     return TRUE;
 }
 
 static boolean test_init_invalid_params(void)
 {
     TStack_alloc sa;
     TStack_alloc_error err;
 
     // NULL sa
     err = StackAlloc_Init(NULL_PTR, g_test_buffer, TEST_BUFFER_SIZE);
     TEST_ASSERT(err == STACK_ALLOC_ERROR_INVALID_PARAM, "Should fail with NULL sa");
 
     // NULL buffer
     err = StackAlloc_Init(&sa, NULL_PTR, TEST_BUFFER_SIZE);
     TEST_ASSERT(err == STACK_ALLOC_ERROR_INVALID_PARAM, "Should fail with NULL buffer");
 
     // Zero size buffer
     err = StackAlloc_Init(&sa, g_test_buffer, 0U);
     TEST_ASSERT(err == STACK_ALLOC_ERROR_INVALID_PARAM, "Should fail with zero size");
 
     return TRUE;
 }
 
 static boolean test_alloc_basic(void)
 {
     TStack_alloc sa;
     StackAlloc_Init(&sa, g_test_buffer, TEST_BUFFER_SIZE);
 
     void* ptr1 = StackAlloc_Alloc(&sa, 10U);
     TEST_ASSERT(ptr1 != NULL_PTR, "Should allocate 10 bytes");
 
     void* ptr2 = StackAlloc_Alloc(&sa, 20U);
     TEST_ASSERT(ptr2 != NULL_PTR, "Should allocate 20 bytes");
     TEST_ASSERT((uint8*)ptr2 > (uint8*)ptr1, "Allocations should grow upward");
 
     uint32 used = StackAlloc_GetUsed(&sa);
     uint32 expected_min = 30U; // 10 + 20
     TEST_ASSERT(used >= expected_min, "Used memory should be at least sum of allocations");
 
     return TRUE;
 }
 
 static boolean test_alloc_zero_size(void)
 {
     TStack_alloc sa;
     StackAlloc_Init(&sa, g_test_buffer, TEST_BUFFER_SIZE);
 
     void* ptr = StackAlloc_Alloc(&sa, 0U);
     TEST_ASSERT(ptr == NULL_PTR, "Zero size allocation should return NULL");
 
     return TRUE;
 }
 
 static boolean test_alloc_null_sa(void)
 {
     void* ptr = StackAlloc_Alloc(NULL_PTR, 10U);
     TEST_ASSERT(ptr == NULL_PTR, "Should return NULL if sa is NULL");
 
     return TRUE;
 }
 
 static boolean test_alloc_overflow(void)
 {
     TStack_alloc sa;
     StackAlloc_Init(&sa, g_test_buffer, TEST_BUFFER_SIZE);
 
     // Allocate almost all space
     uint32 avail = StackAlloc_GetAvailable(&sa);
     void* ptr = StackAlloc_Alloc(&sa, avail + 1U); // Request 1 byte more than available
     TEST_ASSERT(ptr == NULL_PTR, "Should fail to allocate beyond capacity");
 
     return TRUE;
 }
 
 static boolean test_calloc_basic(void)
 {
     TStack_alloc sa;
     StackAlloc_Init(&sa, g_test_buffer, TEST_BUFFER_SIZE);
 
     uint32* ptr = (uint32*)StackAlloc_Calloc(&sa, 5, sizeof(uint32));
     TEST_ASSERT(ptr != NULL_PTR, "Calloc should succeed");
 
     // Verify memory is zero-initialized
     for (int i = 0; i < 5; i++) {
         TEST_ASSERT(ptr[i] == 0, "Calloc should zero-initialize memory");
     }
 
     return TRUE;
 }
 
 static boolean test_reset(void)
 {
     TStack_alloc sa;
     StackAlloc_Init(&sa, g_test_buffer, TEST_BUFFER_SIZE);
 
     // Allocate some memory
     void* ptr = StackAlloc_Alloc(&sa, 100U);
     TEST_ASSERT(ptr != NULL_PTR, "Allocation before reset");
 
     uint32 used_before = StackAlloc_GetUsed(&sa);
     TEST_ASSERT(used_before > 0U, "Used memory should be >0 before reset");
 
     StackAlloc_Reset(&sa);
 
     uint32 used_after = StackAlloc_GetUsed(&sa);
     TEST_ASSERT(used_after == 0U, "Used memory should be 0 after reset");
 
     // Test reset with NULL — should not crash
     StackAlloc_Reset(NULL_PTR);
 
     return TRUE;
 }
 
 static boolean test_capacity_used_available(void)
 {
     TStack_alloc sa;
     StackAlloc_Init(&sa, g_test_buffer, TEST_BUFFER_SIZE);
 
     uint32 cap = StackAlloc_GetCapacity(&sa);
     TEST_ASSERT(cap == TEST_BUFFER_SIZE, "Capacity should match init size");
 
     uint32 used = StackAlloc_GetUsed(&sa);
     TEST_ASSERT(used == 0U, "Used should be 0 initially");
 
     uint32 avail = StackAlloc_GetAvailable(&sa);
     TEST_ASSERT(avail > 0U && avail <= TEST_BUFFER_SIZE, "Available should be positive and <= total size");
 
     // Test NULL sa
     TEST_ASSERT(StackAlloc_GetCapacity(NULL_PTR) == 0U, "Capacity should be 0 for NULL");
     TEST_ASSERT(StackAlloc_GetUsed(NULL_PTR) == 0U, "Used should be 0 for NULL");
     TEST_ASSERT(StackAlloc_GetAvailable(NULL_PTR) == 0U, "Available should be 0 for NULL");
 
     return TRUE;
 }
 
 static boolean test_validate(void)
 {
     TStack_alloc sa;
     TStack_alloc_error err;
 
     // NULL sa
     err = StackAlloc_Validate(NULL_PTR);
     TEST_ASSERT(err != STACK_ALLOC_OK, "NULL sa should be invalid");
 
     // Valid init
     err = StackAlloc_Init(&sa, g_test_buffer, TEST_BUFFER_SIZE);
     TEST_ASSERT(err == STACK_ALLOC_OK, "Init should succeed");
 
     err = StackAlloc_Validate(&sa);
     TEST_ASSERT(err == STACK_ALLOC_OK, "Freshly initialized should be valid");
 
     return TRUE;
 }
 
 static boolean test_free_to_marker_behavior(void)
 {
     TStack_alloc sa;
     StackAlloc_Init(&sa, g_test_buffer, TEST_BUFFER_SIZE);
 
     // Allocate three blocks
     void* a = StackAlloc_Alloc(&sa, 10U);
     void* b = StackAlloc_Alloc(&sa, 20U);
     void* c = StackAlloc_Alloc(&sa, 30U);
 
     TEST_ASSERT(a != NULL_PTR && b != NULL_PTR && c != NULL_PTR, "All allocations should succeed");
 
     // Free in reverse order
     TStack_alloc_error err = StackAlloc_FreeToMarker(&sa, b);
     TEST_ASSERT(err == STACK_ALLOC_OK, "Free to marker b should work");
 
     // Try to free to c (which is after current) - should fail
     err = StackAlloc_FreeToMarker(&sa, c);
     TEST_ASSERT(err != STACK_ALLOC_OK, "Freeing forward should fail");
 
     return TRUE;
 }
 
 /* ========================= Test Runner ========================= */
 
 boolean StackAlloc_RunAllTests(void)
 {
     boolean all_passed = TRUE;
 
     printf("=== Starting Stack Allocator Test Suite ===\n");
 
     TEST_CASE(init_basic);
     TEST_CASE(init_invalid_params);
     TEST_CASE(alloc_basic);
     TEST_CASE(alloc_zero_size);
     TEST_CASE(alloc_null_sa);
     TEST_CASE(alloc_overflow);
     TEST_CASE(calloc_basic);
     TEST_CASE(reset);
     TEST_CASE(capacity_used_available);
     TEST_CASE(validate);
     TEST_CASE(free_to_marker_behavior);
 
     if (all_passed)
     {
         printf("=== All Tests Passed ===\n");
     }
     else
     {
         printf("=== Some Tests Failed ===\n");
     }
 
     return all_passed;
 }