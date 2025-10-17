// timer.c
#include "idt/timer.h"
#include "idt/interrupts.h"
#include "idt/ports.h"

#include "std.h"

uint32_t tick = 0;

// This is the C-level handler for the timer interrupt (IRQ0)
void timer_handler(registers_t regs)
{
    (void)regs;
    tick++;
    // You could do something every N ticks, like print to the screen
    if (tick % 100 == 0)
    {
        std_print("Tick! \n");
    }
}

void init_timer(uint32_t frequency)
{
    // The PIT's input frequency is roughly 1.193180 MHz
    uint32_t divisor = 1193180 / frequency;

    // Send the command byte 0x36
    // This sets the PIT to repeating mode (Square Wave) on channel 0
    port_byte_out(0x43, 0x36);

    // Send the divisor in two parts, low byte then high byte
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}