#include "std.h"

/*
 * The main kernel entry point.
 */
void kmain()
{
  std_clear_screen();
  std_print("Welcome to my tiny OS 123!\n");
  std_print("This is a new line.\n");
  std_print("This is a very long line that should wrap around to the next line automatically if it is long enough to exceed the screen width of 80 characters.");
}
