#include "gdt/gdt.h"
#include "idt/idt.h"
#include "idt/pic.h"
#include "idt/timer.h"
#include "lib/std.h"

#include "kmalloc_simple.h"
#include "process/process.h"

// --- Global "turn" variable ---
// 1 = Task A, 2 = Task B, 3 = Task C
volatile int turn = 1;

// --- Task A (PID 1) ---
void task_a_main()
{
    std_print("Process A (PID 1) started.\n");
    int i = 1;
    for (;;)
    {
        // Wait for Turn 1
        while (turn != 1)
            ;

        std_print("[pid 1]: %d\n", i);

        // Pass turn to Task B (2)
        turn = 2;
        i++;
    }
}

// --- Task B (PID 2) ---
void task_b_main()
{
    std_print("Process B (PID 2) started.\n");
    int i = 1;
    for (;;)
    {
        // Wait for Turn 2
        while (turn != 2)
            ;

        std_print("[pid 2]: %d\n", i);

        // CRITICAL CHANGE: Pass turn to Task C (3) instead of A
        turn = 3;
        i++;
    }
}

// --- Task C (PID 3) ---
void task_c_main()
{
    std_print("Process C (PID 3) started.\n");
    int i = 1;
    for (;;)
    {
        // Wait for Turn 3
        while (turn != 3)
            ;

        std_print("[pid 3]: %d\n", i);

        // CRITICAL CHANGE: Pass turn back to Task A (1) to close the loop
        turn = 1;
        i++;
    }
}

// --- Kernel Main ---
void kmain()
{
    // 1. Disable Interrupts
    asm volatile("cli");

    // 2. Initialize Hardware
    init_gdt();
    remap_pic();
    init_idt();

    // 3. Initialize Software
    init_kheap();
    init_tasking();

    // 4. Create Tasks (Now including C)
    create_process(&task_a_main);
    create_process(&task_b_main);
    create_process(&task_c_main); // <--- Added Task C

    std_print("Tasks created. Halting now to check stability.\n");
    while (1)
        ;

    // 5. Initialize Timer
    init_timer(100);

    // 6. Boot Messages
    std_clear_screen();
    std_print("Kernel Started with 3 Concurrent Processes.\n");
    std_print("Tasks A, B, and C created.\n");

    // 7. Enable Interrupts
    std_print("Entering idle loop...\n");
    asm volatile("sti");

    // 8. Idle Loop
    for (;;)
    {
        asm volatile("hlt");
    }
}