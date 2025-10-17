// process.c

#include "process.h"
#include "define.h"
#include "std.h"

#define MAX_PROCESSES 16

// --- GLOBAL VARIABLE DEFINITIONS ---
// Store all PCB on your system
static pcb_t    pcb_table[MAX_PROCESSES];
static uint8_t  process_stacks[MAX_PROCESSES][4096]; // 4KB stack per process
static pcb_t*   ready_queue = NULL;
static uint32_t next_pid    = 1;

// This is the one non-static variable, declared 'extern' in the header.
pcb_t* current_process = NULL;

void process_init()
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        pcb_table[i].process_state = TERMINATED;
    }
    ready_queue     = NULL;
    current_process = NULL;
    next_pid        = 1;
}

void create_process(void (*entry_point)())
{
    pcb_t* pcb = NULL;

    // Find an empty slot in the process table
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        std_print("Checking slot %d/%d\n", i, MAX_PROCESSES);
        if (pcb_table[i].process_state == TERMINATED)
        {
            pcb = &pcb_table[i];
            std_print("Slot %d free!\n", i, MAX_PROCESSES);
            break;
        }
    }
    if (pcb == NULL)
    {
        // No free processes! Handle error.
        std_print("All slot busy! (%d/%d)", MAX_PROCESSES, MAX_PROCESSES);
        return;
    }

    // Assign a PID and set up the stack pointer
    pcb->pid            = next_pid++;
    uint32_t* stack_ptr = (uint32_t*)&process_stacks[pcb->pid - 1][4095];

    // --- Create a fake interrupt stack frame ---
    *(--stack_ptr) = 0x202;                 // EFLAGS (interrupts enabled)
    *(--stack_ptr) = 0x08;                  // CS (kernel code segment)
    *(--stack_ptr) = (uint32_t)entry_point; // EIP (the process's starting function)

    // pusha registers (eax, ecx, edx, ebx, esp, ebp, esi, edi)
    *(--stack_ptr) = 0; // eax
    *(--stack_ptr) = 0; // ecx
    *(--stack_ptr) = 0; // edx
    *(--stack_ptr) = 0; // ebx
    *(--stack_ptr) = 0; // esp_dummy
    *(--stack_ptr) = 0; // ebp
    *(--stack_ptr) = 0; // esi
    *(--stack_ptr) = 0; // edi

    // Set the PCB's state
    pcb->cpu_state.esp = (uint32_t)stack_ptr;
    pcb->process_state = READY;

    // Add to the circular ready queue
    if (ready_queue == NULL)
    {
        ready_queue = pcb;
        pcb->next   = pcb;
    }
    else
    {
        pcb->next         = ready_queue->next;
        ready_queue->next = pcb;
    }
}