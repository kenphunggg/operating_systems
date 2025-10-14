// include/ports.h
#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

// Read a byte from a port
uint8_t port_byte_in(uint16_t port);

// Write a byte to a port
void port_byte_out(uint16_t port, uint8_t data);

#endif