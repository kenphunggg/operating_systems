// src/idt/idt.c
#include "idt/idt.h"
#include "lib/std.h"

// --- 1. Declare ALL 33 ISR stubs ---
// (You must have all 33 of these in interrupt_stubs.s)
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32(); // Timer

// The IDT
idt_entry_t idt_entries[256];
// The IDT pointer
idt_ptr_t idt_ptr;

// Helper to set a gate
static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_low  = (base & 0xFFFF);
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].sel       = sel;
    idt_entries[num].always0   = 0;
    idt_entries[num].flags     = flags;
}

// IDT Initialization
void init_idt()
{
    // Set up the IDT pointer
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    // Clear the IDT
    unsigned char* idt_ptr_byte = (unsigned char*)&idt_entries;
    for (unsigned int i = 0; i < sizeof(idt_entry_t) * 256; ++i)
    {
        idt_ptr_byte[i] = 0;
    }

    // --- 2. Create an array of all your ISR functions ---
    void (*isr_stub_table[33])() = {isr0,  isr1,  isr2,  isr3,  isr4,  isr5,  isr6,  isr7,  isr8,
                                    isr9,  isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17,
                                    isr18, isr19, isr20, isr21, isr22, isr23, isr24, isr25, isr26,
                                    isr27, isr28, isr29, isr30, isr31, isr32};

    // --- 3. Use a loop to set all 33 gates ---
    // This is much safer than doing it manually.
    for (int i = 0; i < 33; i++)
    {
        // 0x08 is Kernel Code Segment
        // 0x8E is Present, Ring 0, 32-bit Interrupt Gate
        idt_set_gate(i, (uint32_t)isr_stub_table[i], 0x08, 0x8E);
    }

    // Load the IDT
    idt_flush((uint32_t)&idt_ptr);
}