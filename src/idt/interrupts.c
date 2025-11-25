// src/interrupts.c
#include "idt/interrupts.h"
#include "idt/ports.h" // For port_byte_out
#include "idt/timer.h" // <-- This has the correct declaration
#include "process/process.h"

#include "lib/std.h"

// --- BUG 1: DELETE THIS LINE ---
// This forward declaration is incorrect and conflicts with timer.h
// void timer_handler(registers_t regs);

extern process_control_block_t* current_task;

// Main interrupt handler and dispatcher
uint32_t interrupt_handler(registers_t* regs)
{
    // 1. Save the old task's stack pointer
    if (current_task)
    {
        current_task->esp = (uint32_t)regs;
    }

    // 2. Handle IRQs (Hardware Interrupts)
    if (regs->int_no >= 32)
    {
        // Handle Timer (IRQ0)
        if (regs->int_no == 32)
        {
            timer_handler(regs);
        }

        // Send EOI to PIC (Must happen before we might switch tasks)
        port_byte_out(0x20, 0x20);
        if (regs->int_no >= 40)
        {
            port_byte_out(0xA0, 0x20);
        }

        // 3. Call Scheduler
        // We do this AFTER the specific handlers finish.
        // This ensures the timer has updated the 'tick' count before we switch.
        schedule();
    }
    else
    {
        // --- Handle CPU Exception ---
        std_print("Received exception: %d\n", regs->int_no);
        std_print("Error Code: %x\n", regs->err_code); // Helpful to print this too
        std_print("CPU Halted.\n");
        for (;;)
            asm volatile("hlt");
    }

    // 4. Return the NEW task's stack pointer
    return current_task->esp;
}