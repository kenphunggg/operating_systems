// process.h
#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h> // For types like uint32_t

// This structure defines the state of the CPU registers.
// We'll use this to save and restore the context of a process.
// 1. edi, esi, ebp, esp, ebx, edx, ecx, eax is 8-32bit can be use for any task
// 1.1 eax: Accumulator Register. Often used for arithmetic operations and
// storing the return value from functions. 1.2 ebx: Base Register. Typically
// used as a base pointer for addressing memory. 1.3 ecx: Counter Register. Used
// as a loop counter. 1.4 edx: Data Register. Often used for I/O operations and
// in conjunction with eax for multiplication and division. 1.5 esi: Source
// Index. Used as a source pointer in string and memory operations. 1.6 edi:
// Destination Index. Used as a destination pointer in string and memory
// operations. 1.7 ebp: Base Pointer. Used to point to the base of the current
// stack frame, allowing access to local variables and function arguments. 1.8
// esp: Stack Pointer. Always points to the top of the current stack. 2. 2.1
// eip: Its one and only job is to hold the memory address of the next
// instruction the CPU will execute 2.2 cs: Doesn't point directly to memory but
// instead holds an index into a GDT 2.3 eflags: CPU's status and control
// dashboard
typedef struct
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t eip, cs, eflags;                        // Pushed by the CPU on interrupt
} cpu_state_t;

// An enum to represent the state of a process.
typedef enum
{
    NEW,
    READY,
    RUNNING,
    WAITING,
    TERMINATED,
} process_state_e;

// The Process Control Block (PCB)
typedef struct pcb
{
    process_state_e process_state; // Current state of the process
    cpu_state_t     cpu_state;     // The saved CPU state
    uint32_t        pid;           // Process IDFC
    struct pcb*     next;
} pcb_t;

void process_init();

void create_process(void (*entry_point)());

#endif // PROCESS_H
