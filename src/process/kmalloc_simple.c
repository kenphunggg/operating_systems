#include "kmalloc_simple.h"
#include "lib/std.h"

// Symbol defined in linker.ld
extern uint32_t _end;
static uint32_t heap_ptr;

// Initialize kernel heap
void init_kheap()
{
    // Start the heap right after the kernel's .bss section
    heap_ptr = (uint32_t)&_end;

    // Align to 4K page boundary for safety
    if (heap_ptr & 0xFFF)
    {
        heap_ptr = (heap_ptr & 0xFFFFF000) + 0x1000;
    }
    std_print("Simple heap initialized.\n");
}

// Very simple bump-allocator (does not support free)
void* kmalloc(uint32_t size)
{
    uint32_t addr = heap_ptr;
    heap_ptr += size;
    return (void*)addr;
}