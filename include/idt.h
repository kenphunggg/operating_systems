// idt.h
#ifndef IDT_H_
#define IDT_H_

#include <stdint.h>

// Defines a single IDT entry (Interrupt Gate).
// The __attribute__((packed)) is crucial to prevent the compiler from adding padding.
struct idt_entry_struct
{
    uint16_t base_low;  // The lower 16 bits of the address to jump to.
    uint16_t sel;       // Kernel segment selector.
    uint8_t always0;    // This must always be zero.
    uint8_t flags;      // More flags. See documentation.
    uint16_t base_high; // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

// The IDT pointer structure, which is loaded into the IDTR register.
struct idt_ptr_struct
{
    uint16_t limit; // Size of the IDT minus 1.
    uint32_t base;  // Linear address of the IDT.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// Our main IDT initialization function.
void init_idt();

// This function is defined in assembly. It loads the IDT pointer into the IDTR register.
extern void idt_flush(uint32_t);

#endif // IDT_H_