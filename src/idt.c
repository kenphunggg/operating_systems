// idt.c
#include "idt.h"
#include "std.h" // For printing

// Declare the ISR stubs from interrupts.s
extern void isr0();
extern void isr1();
extern void isr2();
// ... and so on for all 32
extern void isr31();
extern void isr32(); // Interrupt vector 32 for PIC

// The Interrupt Descriptor Table itself. We will handle 256 interrupts.
idt_entry_t idt_entries[256];

// The pointer to the IDT.
idt_ptr_t idt_ptr;

// Helper function to create an IDT gate.
static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_low = (base & 0xFFFF);
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags = flags /* | 0x60 */;
}

// IDT Initialization function.
void init_idt()
{
    // Set up the IDT pointer.
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    // Clear the IDT by filling it with zeros.
    unsigned char *idt_ptr_byte = (unsigned char *)&idt_entries;
    for (unsigned int i = 0; i < sizeof(idt_entry_t) * 256; ++i)
    {
        idt_ptr_byte[i] = 0;
    }

    // Set up the first 32 ISRs (Intel-defined exceptions).
    // 0x08 is our kernel code segment selector.
    // 0x8E sets the gate type to a 32-bit interrupt gate and present.
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
    // ... continue for all 32 ISRs
    // A loop or macro would be better in a real project!
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);
    idt_set_gate(32, (uint32_t)isr32, 0x08, 0x8E); // Interrupt vector 32 for PIC

    // Load the IDT using our assembly function.
    idt_flush((uint32_t)&idt_ptr);

    std_print("IDT Initialized.\n");
}