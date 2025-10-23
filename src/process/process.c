#include "process/process.h" // <-- The header you provided
#include "kmalloc_simple.h"  // For kmalloc
#include "lib/define.h"
#include "lib/std.h" // For std_print

#define MAX_TASKS 16
#define KERNEL_STACK_SIZE 4096

// --- GLOBAL VARIABLE DEFINITIONS ---

// Use the types from process.h
static task_struct_t* task_list[MAX_TASKS] = {0};

// This is the one non-static variable, declared 'extern' in the header.
task_struct_t* current_task = 0;

// Internal scheduler variables
static uint32_t current_task_index = 0;
static uint32_t next_pid           = 0;

// Initializes the scheduler and creates the kernel task
void init_tasking()
{
    // 1. Allocate a PCB for the kernel task (Task 0)
    task_struct_t* kernel_task = (task_struct_t*)kmalloc(sizeof(task_struct_t));
    kernel_task->id            = next_pid++;   // id = 0
    kernel_task->state         = TASK_RUNNING; // It's already running
    kernel_task->esp           = 0;            // Will be set by interrupt_handler on first switch

    // 2. Add to list and set as current
    task_list[0]       = kernel_task;
    current_task       = kernel_task;
    current_task_index = 0;

    std_print("Tasking initialized.\n");
}

// Creates a new process that will run 'entry'
void create_process(void (*entry)())
{
    // 1. Find a free slot in the task list
    int i = 0;
    while (task_list[i] != 0 && i < MAX_TASKS)
    {
        i++;
    }
    if (i == MAX_TASKS)
    {
        std_print("PANIC: Max tasks reached!\n");
        return;
    }

    // --- State: TASK_NEW ---

    // 2. Allocate and set up the PCB
    task_struct_t* new_task = (task_struct_t*)kmalloc(sizeof(task_struct_t));
    new_task->id            = next_pid++;
    new_task->state         = TASK_NEW; // Use the enum from process.h

    // 3. Allocate a kernel stack
    uint8_t*  stack_base = (uint8_t*)kmalloc(KERNEL_STACK_SIZE);
    uint32_t* stack_ptr  = (uint32_t*)(stack_base + KERNEL_STACK_SIZE);

    // --- Create a fake interrupt stack frame ---
    // This must match the 'iret' and 'popa' from your stub

    // 1. IRET Frame (popped by CPU)
    *(--stack_ptr) = 0x0202;          // EFLAGS (interrupts enabled)
    *(--stack_ptr) = 0x08;            // CS (kernel code segment)
    *(--stack_ptr) = (uint32_t)entry; // EIP

    // 2. Interrupt Frame (popped by 'add esp, 8' in stub)
    *(--stack_ptr) = 0; // dummy error code
    *(--stack_ptr) = 0; // dummy int_no

    // 3. 'pusha' Frame (popped by 'popa' in stub)
    // Pushed in REVERSE order (EAX first, EDI last)
    *(--stack_ptr) = 0; // EAX
    *(--stack_ptr) = 0; // ECX
    *(--stack_ptr) = 0; // EDX
    *(--stack_ptr) = 0; // EBX
    *(--stack_ptr) = 0; // esp_dummy
    *(--stack_ptr) = 0; // EBP
    *(--stack_ptr) = 0; // ESI
    *(--stack_ptr) = 0; // EDI

    // Set the PCB's state
    // 'esp' now points to the top of our fake stack (at EDI)
    new_task->esp = (uint32_t)stack_ptr;

    // 7. Add to task list and set state
    task_list[i]    = new_task;
    new_task->state = TASK_READY; // Use the enum from process.h

    std_print("Created process %d\n", new_task->id);
}

// The main scheduler function, called by the timer
void schedule()
{
    // This function just *picks* the next task.
    // The assembly stub does the *switch*.

    task_struct_t* prev_task = current_task;

    // 1. If the current task was running, set it back to READY
    if (prev_task->state == TASK_RUNNING)
    {
        prev_task->state = TASK_READY;
    }

    // 2. Find the next READY task (simple round-robin)
    uint32_t i = current_task_index;
    while (1)
    {
        i = (i + 1) % MAX_TASKS;

        // Found a valid, ready task?
        if (task_list[i] && task_list[i]->state == TASK_READY)
        {
            // 3. Set it as the new current task
            current_task_index  = i;
            current_task        = task_list[i];
            current_task->state = TASK_RUNNING;

            // The interrupt handler will return this new 'current_task->esp'
            return;
        }

        // 4. If we looped all the way around...
        if (i == current_task_index)
        {
            // No other task is READY.
            // If the current (previous) task is still READY, run it.
            if (prev_task->state == TASK_READY)
            {
                prev_task->state = TASK_RUNNING;
                current_task     = prev_task;
            }
            // If the prev_task is WAITING or TERMINATED,
            // 'current_task' won't change, and the interrupt handler
            // will just return the ESP of that (hlt'ing) task.
            return;
        }
    }
}

// --- Process State Functions (Stubbed) ---
// (These will be implemented fully when you add system calls)

void exit_process()
{
    cli(); // Disable interrupts
    current_task->state = TASK_TERMINATED;
    std_print("Process %d terminated. Halting task.\n", current_task->id);
    // A real exit would free memory and call schedule(),
    // but without a syscall, we just halt.
    for (;;)
        asm volatile("hlt");
}

void block_current_task()
{
    // This function would be called by e.g. read_keyboard()
    // It tells the scheduler to skip this task.
    current_task->state = TASK_WAITING;

    // We must force a schedule().
    // The only *safe* way to do this is via a syscall.
    // For now, we'll just wait for the next timer tick.
    asm volatile("sti; hlt"); // Wait for next interrupt
}

void unblock_task(uint32_t id)
{
    if (id < MAX_TASKS && task_list[id])
    {
        if (task_list[id]->state == TASK_WAITING)
        {
            task_list[id]->state = TASK_READY;
        }
    }
}