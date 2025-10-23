// src/interrupts.c
#include "idt/interrupts.h"
#include "idt/ports.h" // For port_byte_out
#include "idt/timer.h" // <-- This has the correct declaration
#include "process/process.h"

#include "lib/std.h"

// --- BUG 1: DELETE THIS LINE ---
// This forward declaration is incorrect and conflicts with timer.h
// void timer_handler(registers_t regs);

extern task_struct_t* current_task;

// Main interrupt handler and dispatcher
uint32_t interrupt_handler(registers_t* regs)
{
    // 1. Save the old task's stack pointer
    // (If tasking is running)
    if (current_task)
    {
        current_task->esp = (uint32_t)regs;
    }

    // Check if it's an IRQ or an Exception
    if (regs->int_no >= 32)
    {
        // --- Handle Hardware IRQ ---
        if (regs->int_no == 32)
        {
            timer_handler(regs); // Just ticks the clock
        }

        // --- Call the scheduler ---
        // This will pick a new task and update
        // the global 'current_task' variable.
        schedule();

        // Send EOI to PIC
        port_byte_out(0x20, 0x20);
        if (regs->int_no >= 40)
        {
            port_byte_out(0xA0, 0x20);
        }
    }
    else
    {
        // --- Handle CPU Exception ---
        std_print("Received exception: %d\n", regs->int_no);
        std_print("CPU Halted.\n");
        // We use 'for(;;)' here because we can't recover
        for (;;)
            asm volatile("hlt");
    }

    // 3. Return the NEW task's stack pointer to the assembly stub
    return current_task->esp;
}