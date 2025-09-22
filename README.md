# Stack Allocator

A simple, fast, and deterministic memory allocator that operates on a pre-allocated buffer in a LIFO (Last-In-First-Out) manner. Designed for embedded systems and real-time applications where memory allocation patterns are predictable.

## Features

- **Deterministic** allocation and deallocation in O(1) time
- **Memory safety** through boundary checking and state validation
- **Thread-unsafe** by design (for simplicity, add mutexes if needed)
- **Minimal overhead** with constant-time operations
- **No external dependencies** beyond standard C libraries

## Project Structure

```
StackAllocator/
├── base/            # Base utilities and type definitions
├── cfg/             # Configuration files
├── demo/            # Example usage
└── src/             # Core implementation
```

## API Reference

### Initialization

```c
TStack_alloc_error StackAlloc_Init(TStack_alloc* sa, void* buffer, uint32 buffer_size);
```
Initialize a stack allocator with a pre-allocated buffer.

### Memory Allocation

```c
void* StackAlloc_Alloc(TStack_alloc* sa, uint32 size);
```
Allocate a block of memory from the stack.

```c
void* StackAlloc_Calloc(TStack_alloc* sa, uint32 num, uint32 size);
```
Allocate and zero-initialize an array of elements.

### Memory Management

```c
TStack_alloc_error StackAlloc_FreeToMarker(TStack_alloc* sa, void* marker);
```
Free memory back to a previously obtained marker.

```c
void StackAlloc_Reset(TStack_alloc* sa);
```
Reset the allocator, freeing all allocated memory.

### Query Functions

```c
uint32 StackAlloc_GetCapacity(const TStack_alloc* sa);
uint32 StackAlloc_GetUsed(const TStack_alloc* sa);
uint32 StackAlloc_GetAvailable(const TStack_alloc* sa);
TStack_alloc_error StackAlloc_Validate(const TStack_alloc* sa);
```

## Usage Example

```c
#include "stack_alloc.h"
#include <stdio.h>

#define BUFFER_SIZE 1024

int main() {
    // 1. Create a memory buffer
    uint8 buffer[BUFFER_SIZE];
    
    // 2. Initialize the stack allocator
    TStack_alloc sa;
    TStack_alloc_error err = StackAlloc_Init(&sa, buffer, BUFFER_SIZE);
    if (err != STACK_ALLOC_OK) {
        printf("Initialization failed\n");
        return -1;
    }
    
    // 3. Allocate some memory
    int* numbers = (int*)StackAlloc_Alloc(&sa, 10 * sizeof(int));
    if (numbers == NULL) {
        printf("Allocation failed\n");
        return -1;
    }
    
    // 4. Use the allocated memory
    for (int i = 0; i < 10; i++) {
        numbers[i] = i * i;
    }
    
    // 5. Get a marker to free back to later
    void* marker = StackAlloc_GetMarker(&sa);
    
    // 6. Allocate more memory
    char* message = (char*)StackAlloc_Alloc(&sa, 100);
    if (message != NULL) {
        snprintf(message, 100, "Hello, Stack Allocator!");
        printf("%s\n", message);
    }
    
    // 7. Free memory back to the marker (frees 'message' but keeps 'numbers')
    err = StackAlloc_FreeToMarker(&sa, marker);
    if (err != STACK_ALLOC_OK) {
        printf("Failed to free memory: %d\n", err);
    }
    
    // 8. Reset everything
    StackAlloc_Reset(&sa);
    
    return 0;
}
```

## Building the Project

```bash
# Clone the repository
git clone https://github.com/gradinarovo/StackAllocator.git
cd StackAllocator

# Build the project
mingw32-make

# Run tests
mingw32-make test

# Clean build artifacts
mingw32-make clean
```

## Error Handling

The stack allocator returns error codes for various failure conditions:

- `STACK_ALLOC_OK`: Operation completed successfully
- `STACK_ALLOC_ERROR_INVALID_PARAM`: Invalid parameter passed to function
- `STACK_ALLOC_ERROR_OUT_OF_MEMORY`: Not enough memory available
- `STACK_ALLOC_ERROR_CORRUPTED_STATE`: Memory corruption detected
- `STACK_ALLOC_ERROR_INVALID_MARKER`: Invalid marker or stack position
- `STACK_ALLOC_ERROR_NOT_LIFO`: Free operation violates LIFO order

## Performance Characteristics

- **Allocation**: O(1) - Constant time
- **Deallocation**: O(1) - Constant time (only LIFO deallocation supported)
- **Memory overhead**: Minimal (just a few bytes per allocator instance)

## License

Copyright 2025 Ivan Mateev

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
