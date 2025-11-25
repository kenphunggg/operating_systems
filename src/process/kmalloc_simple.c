#include "kmalloc_simple.h"
#include "lib/std.h"

// We ignore _end for now to prevent stack collisions.
// We will force the heap to start at 4MB (0x400000).
// This ensures we don't overwrite the Kernel (at 1MB) or the Stack.
static uint32_t heap_ptr = 0x400000;

void init_kheap()
{
    // No need to calculate from &_end anymore.
    // Just ensure we are page aligned (which 0x400000 is).
    std_print("Simple heap initialized at 0x400000.\n");
}

void* kmalloc(uint32_t size)
{
    // 1. Save the current address
    uint32_t addr = heap_ptr;

    // 2. Increment the pointer
    heap_ptr += size;

    // 3. Optional: Align the next allocation to 4 bytes for performance
    if (heap_ptr % 4 != 0)
    {
        heap_ptr += (4 - (heap_ptr % 4));
    }

    // 4. Return the address
    return (void*)addr;
}