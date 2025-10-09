#include "std.h"
#include "gdt.h"

/*
 * The main kernel entry point.
 */
void kmain()
{
  std_print("Initializing GDT...\n");
  init_gdt();
  std_print("GDT Initialized.\n");
}
