; interrupts.asm
bits 32

; This macro creates a common ISR stub.
; It pushes an error code if one exists, then pushes the interrupt number,
; and finally jumps to a common C-level interrupt handler.
%macro ISR_NOERRCODE 1
  global isr%1
  isr%1:
    cli           ; Disable interrupts
    push 0        ; Push a dummy error code
    push %1       ; Push the interrupt number
    jmp isr_common_stub
%endmacro

; Macro for ISRs that have an error code pushed by the CPU.
%macro ISR_ERRCODE 1
  global isr%1
  isr%1:
    cli           ; Disable interrupts
    push %1       ; Push the interrupt number
    jmp isr_common_stub
%endmacro

; Create stubs for the first 32 interrupts (ISRs).
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_ERRCODE   30
ISR_NOERRCODE 31
ISR_NOERRCODE 32 ; Interrupt vector 32 for PIC

; Common stub that all ISRs jump to.
; This saves the processor state and calls the C-level handler.
extern interrupt_handler
isr_common_stub:
    pusha           ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, 0x10    ; Load the kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; --- THIS IS THE FIX ---
    ; 'esp' is now pointing to the 'pusha' frame.
    ; This is the 'registers_t* regs' pointer we need.
    push esp        ; Pass 'esp' as the C function's argument

    call interrupt_handler

    add esp, 4      ; Clean up the argument we pushed
    ; ----------------------

    ; --- THIS IS THE CONTEXT SWITCH ---
    mov esp, eax    ; Load the new task's stack pointer

    popa
    add esp, 8      ; Cleans up the error code and ISR number
    iret



; This function loads the IDT pointer into the IDTR register.
global idt_flush
idt_flush:
    mov eax, [esp+4]    ; Get the pointer from the stack
    lidt [eax]          ; Load the IDT pointer
    ret