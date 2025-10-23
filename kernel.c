// kernel.c
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "idt/pic.h"
#include "idt/timer.h"
#include "lib/std.h"

#include "kmalloc_simple.h"
#include "process/process.h"

// --- 1. Create a global "turn" variable ---
// 'volatile' is critical: it tells the compiler
// that this value can change at any time,
// so it must not optimize away our 'while' loops.
volatile int turn = 1;

// Test function for Process A
void task_a_main()
{
    std_print("Process A (Task 1) started.\n");
    int i = 1;
    for (;;)
    {
        // Wait (spin) until it's our turn
        while (turn != 1)
        {
            // Do nothing, just wait.
            // The timer interrupt will still fire
            // and switch to Task B.
        }

        // It's our turn!
        std_print("task1: %d\n", i);

        // Pass the turn to Task B
        turn = 2;
        i++;
    }
}

// --- 3. Update Task B ---
void task_b_main()
{
    std_print("Process B (Task 2) started.\n");
    int i = 1;
    for (;;)
    {
        // Wait (spin) until it's our turn
        while (turn != 2)
        {
            // Do nothing, just wait.
        }

        // It's our turn!
        std_print("task2: %d\n", i);

        // Pass the turn back to Task A
        turn = 1;
        i++;
    }
}

// Kernel Main
void kmain()
{
    // --- 1. CRITICAL: Ensure interrupts are OFF ---
    // This is the "master switch" for initialization.
    asm volatile("cli");

    // --- 2. Initialize Hardware ---
    init_gdt();
    remap_pic();
    init_idt(); // IDT is loaded, but interrupts are off

    // --- 3. Initialize Software Systems ---
    init_kheap();
    init_tasking(); // Creates kernel task (Task 0)

    // --- 4. Create Tasks ---
    create_process(&task_a_main);
    create_process(&task_b_main);

    // --- 5. Initialize Timer ---
    init_timer(100); // 100 Hz

    // --- 6. Now, print all boot messages ---
    // (This is safe, as interrupts are still globally OFF.
    // The cli/sti inside std_print won't do anything harmful.)
    std_clear_screen();
    std_print("Kernel Started.\n");
    std_print("GDT Initialized.\n");
    std_print("PIC Remapped.\n");
    std_print("IDT Initialized.\n");
    std_print("Simple heap initialized.\n");
    std_print("Tasking initialized.\n");
    std_print("Created tasks A and B.\n");
    std_print("PIT Initialized at 100 Hz.\n");

    // --- 7. CRITICAL: Enable interrupts NOW ---
    // This is the "start" signal for the scheduler.
    std_print("Entering idle loop. Multitasking enabled.\n");
    asm volatile("sti");

    // --- 8. Kernel Task becomes the "Idle Task" ---
    for (;;)
    {
        asm volatile("hlt"); // Wait for the first interrupt
    }
}