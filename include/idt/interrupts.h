// include/idt/interrupts.h
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

// This struct must match the order of 'pusha'
// and the registers pushed by the isr_stub/CPU.
typedef struct
{
    // Pushed by 'pusha' (in reverse order of popa)
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy; // 'esp' value *before* pusha
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    // Pushed by our ISR stubs (isr%1 and push 0)
    uint32_t int_no;
    uint32_t err_code;

    // Pushed by the CPU automatically on interrupt
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t useresp; // Only valid if CPL changed
    uint32_t ss;      // Only valid if CPL changed
} registers_t;

// The C-level interrupt handler
uint32_t interrupt_handler(registers_t* regs);

#endif