// // kernel.c
// #include "std.h"
// #include "gdt.h"
// #include "idt.h" // Include our new IDT header

// void kmain()
// {
//   std_clear_screen();
//   std_print("Kernel Started.\n");

//   std_print("Initializing GDT...\n");
//   init_gdt();
//   std_print("GDT Initialized.\n");

//   std_print("Initializing IDT...\n");
//   init_idt(); // Initialize the IDT

//   // Trigger a divide-by-zero interrupt to test our IDT.
//   // This will call ISR 0.
//   asm volatile("div %0" ::"r"(0));

//   // This line should not be reached if the interrupt handler works.
//   std_print("This should not be printed!");
// }

// kernel.c
// kernel.c
#include "std.h"
#include "gdt.h"
#include "pic.h" // <-- Include the new header
#include "idt.h"
#include "timer.h"

void kmain()
{
  std_clear_screen();
  std_print("Kernel Started.\n");

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