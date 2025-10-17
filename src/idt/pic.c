// src/pic.c
#include "idt/pic.h"
#include "idt/ports.h"

// I/O Ports for the Master and Slave PICs
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

// Initialization Command Words (ICWs)
#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

void remap_pic()
{
    // Save masks
    uint8_t a1 = port_byte_in(PIC1_DATA);
    uint8_t a2 = port_byte_in(PIC2_DATA);

    // Starts the initialization sequence (in cascade mode)
    port_byte_out(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    port_byte_out(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // ICW2: Master PIC vector offset (start at 32)
    port_byte_out(PIC1_DATA, 32);
    // ICW2: Slave PIC vector offset (start at 40)
    port_byte_out(PIC2_DATA, 40);

    // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    port_byte_out(PIC1_DATA, 4);
    // ICW3: tell Slave PIC its cascade identity (0000 0010)
    port_byte_out(PIC2_DATA, 2);

    // ICW4: have the PICs use 8086 mode
    port_byte_out(PIC1_DATA, ICW4_8086);
    port_byte_out(PIC2_DATA, ICW4_8086);

    // Restore saved masks
    port_byte_out(PIC1_DATA, a1);
    port_byte_out(PIC2_DATA, a2);
}