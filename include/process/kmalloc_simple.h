#ifndef KMALLOC_SIMPLE_H
#define KMALLOC_SIMPLE_H

#include <stdint.h>

// Initialize the heap
void init_kheap();

// Simple, non-freeing memory allocator
void* kmalloc(uint32_t size);

#endif