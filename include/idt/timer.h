// include/timer.h
#ifndef TIMER_H
#define TIMER_H

#include "idt/interrupts.h"
#include <stdint.h>

void timer_handler(registers_t* regs);

void init_timer(uint32_t frequency);

#endif