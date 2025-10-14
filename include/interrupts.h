// interrupts.h
#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <stdint.h>

// Defines the structure of the registers pushed by the ISR stub.
struct registers
{
    uint32_t ds;                                     // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32_t int_no, err_code;                       // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
};
typedef struct registers registers_t;

// The C-level interrupt handler.
void interrupt_handler(registers_t regs);

#endif // INTERRUPTS_H_