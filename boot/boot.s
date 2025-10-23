; boot.s
bits 32
section .multiboot
    ; Your Multiboot header (unchanged)
    dd 0x1BADB002
    dd 0x00
    dd - (0x1BADB002 + 0x00)

; --- ADD THIS SECTION ---
section .bss
align 16
kernel_stack_bottom:
    resb 16384 ; Reserve 16KB for the kernel stack
kernel_stack_top: ; This label is at the top of the stack
; ------------------------

section .text
global _start
extern kmain ; External C function

_start:
    cli          ; Disable interrupts

    ; --- ADD THIS LINE ---
    ; Set the stack pointer to the top of our new stack
    mov esp, kernel_stack_top

    ; Now it's safe to call C
    call kmain   ; Call the C kernel's main function

    hlt          ; Halt the CPU