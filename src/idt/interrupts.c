// src/interrupts.c
#include "idt/interrupts.h"
#include "idt/ports.h" // For port_byte_out
#include "idt/timer.h"

#include "lib/std.h"

// Forward declaration of our timer handler
void timer_handler(registers_t regs);

// Main interrupt handler and dispatcher
void interrupt_handler(registers_t regs)
{
    // Check if this is a hardware interrupt (IRQ)
    if (regs.int_no >= 32 && regs.int_no <= 47)
    {
        // Handle the specific IRQ
        if (regs.int_no == 32)
        {
            timer_handler(regs);
        }

        // Send End-of-Interrupt (EOI) signal to the PIC
        // This is VITAL, or the PIC won't send any more IRQs.
        port_byte_out(0x20, 0x20);

        // If it's a slave PIC interrupt, send to slave PIC as well
        if (regs.int_no >= 40)
        {
            port_byte_out(0xA0, 0x20);
        }
    }
    else
    {
        // It's a CPU exception
        char int_str[10];
        std_print("Received exception: ");
        itoa(regs.int_no, int_str);
        std_print(int_str);
        std_print("\nCPU Halted.\n");
        for (;;)
            ; // Halt
    }
}