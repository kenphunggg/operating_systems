// kernel.c
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "idt/pic.h"
#include "idt/timer.h"
#include "lib/std.h"

void kmain()
{
    std_clear_screen();
    std_print("Kernel Started.\n");
    std_print("This is a very long line. Use for testing print function only. Please contact "
              "github.com/kenphung for more information\n");

    init_gdt();
    std_print("GDT Initialized.\n");

    remap_pic(); // <-- Add this function call
    std_print("PIC Remapped.\n");

    init_idt();

    init_timer(100);
    std_print("PIT Initialized at 100 Hz.\n");

    asm volatile("sti");

    for (;;)
    {
    }
}