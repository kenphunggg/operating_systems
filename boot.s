; Set up the Multiboot header.
bits 32
section .multiboot
    ; Magic number for the Multiboot header.
    dd 0x1BADB002
    ; Flags to indicate that the OS image is 32-bit.
    dd 0x00
    ; Checksum to ensure the header is valid.
    dd - (0x1BADB002 + 0x00)

section .text
global _start
extern kmain ; External C function

_start:
    ; The bootloader (like GRUB) has already put the CPU in 32-bit protected mode.
    ; It has also set up a basic stack. Now, we just need to call our C kernel.
    cli          ; Disable interrupts.
    call kmain   ; Call the C kernel's main function.
    hlt          ; Halt the CPU. Should never be reached.
