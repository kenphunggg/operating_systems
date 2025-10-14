// src/ports.c
#include "ports.h"

uint8_t port_byte_in(uint16_t port)
{
    uint8_t result;
    asm("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

void port_byte_out(uint16_t port, uint8_t data)
{
    asm("out %%al, %%dx" : : "a"(data), "d"(port));
}