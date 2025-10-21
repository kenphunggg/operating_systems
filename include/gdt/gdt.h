#ifndef GDT_H_
#define GDT_H_

#include <stdint.h>

// Defines a single GDT entry. The __attribute__((packed)) is crucial
// to prevent the compiler from adding padding bytes.
struct gdt_entry_struct
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

// The GDT pointer structure, which is loaded into the GDTR register.
struct gdt_ptr_struct
{
    uint16_t limit; // Size of the GDT minus 1.
    uint32_t base;  // Linear address of the GDT.
} __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

// Our main GDT initialization function.
void init_gdt();

#endif // GDT_H_