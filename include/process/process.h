#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

// The 5-state process model
typedef enum {
    TASK_NEW,         // Being created
    TASK_READY,       // Waiting for CPU
    TASK_RUNNING,     // Executing
    TASK_WAITING,     // Blocked on an event
    TASK_TERMINATED   // Finished, needs reaping
} task_state_t;

// The Process Control Block (PCB)
typedef struct task_struct {
    // 'esp' MUST be the first element.
    // context_switch.s depends on this.
    uint32_t esp;       // Kernel Stack Pointer

    uint32_t id;        // Process ID
    task_state_t state; // Current state

    // You will add more here later:
    // - uint32_t* page_directory;
    // - struct task_struct* parent;
    // - int exit_code;

} task_struct_t;

// --- Scheduler Public API ---

// The currently running task
extern task_struct_t* current_task;

// Initializes the scheduler and creates the kernel task
void init_tasking();

// Creates a new process that will run 'entry'
void create_process(void (*entry)());

// The main scheduler function, called by the timer
void schedule();

// --- Process State Functions ---

// Called by a process to terminate itself
void exit_process();

// Called to put the current task to sleep (e.g., waiting for I/O)
void block_current_task();

// Called by an interrupt handler to wake a sleeping task
void unblock_task(uint32_t id);

// Assembly function for switching stacks
extern void context_switch(task_struct_t* prev, task_struct_t* next);

#endif