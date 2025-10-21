#include "gdt/gdt.h"

// The Global Descriptor Table itself.
gdt_entry_t gdt_entries[3];

// The pointer to the GDT.
gdt_ptr_t gdt_ptr;

// Helper function to create a GDT descriptor.
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}

// GDT Initialization function.
void init_gdt()
{
    // Set up the GDT pointer.
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 3) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    // 1. NULL Descriptor (required).
    gdt_set_gate(0, 0, 0, 0, 0);

    // 2. Kernel Code Segment (Ring 0).
    // Base=0, Limit=4GB, Access=Code(Read/Exec), Granularity=4KB pages.
    // Access flags (0x9A): Present(1) DPL(00) Type(1) Code(1) Conforming(0)
    // Readable(1) Accessed(0)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // 3. Kernel Data Segment (Ring 0).
    // Base=0, Limit=4GB, Access=Data(Read/Write), Granularity=4KB pages.
    // Access flags (0x92): Present(1) DPL(00) Type(1) Data(0) Direction(0)
    // Writable(1) Accessed(0)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Load the GDT using inline assembly.
    asm volatile("lgdt %0"
                 :              // No output operands
                 : "m"(gdt_ptr) // Input operand: the GDT pointer in memory
    );

    // Reload segment registers to use the new GDT.
    // We use a "long jump" to reload CS and an immediate move for the others.
    asm volatile("mov $0x10, %%ax\n\t" // 0x10 is the offset of our data segment
                 "mov %%ax, %%ds\n\t"
                 "mov %%ax, %%ss\n\t"
                 "mov %%ax, %%es\n\t"
                 "mov %%ax, %%fs\n\t"
                 "mov %%ax, %%gs\n\t"
                 "ljmp $0x08, $1f\n\t" // 0x08 is the offset of our code segment.
                                       // Jump to a label '1'.
                 "1:"                  // The label '1' is here, right after the jump.
                 :                     // No output operands
                 :                     // No input operands
                 : "eax"               // Clobbers the EAX register
    );
}