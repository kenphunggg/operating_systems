// std.h
#ifndef STD_H_
#define STD_H_

/*
 * integer to ASCII
 */
void itoa(int n, char str[]);

/*
 * Scrolls the screen up by one line.
 */
void scroll();

/*
 * Puts a single character on the screen at the current cursor position.
 * Handles newlines and scrolling.
 * @param c The character to print.
 */
void putchar(char c);

/*
 * Clears the screen by filling it with spaces.
 */
void std_clear_screen();

/*
 * Prints a null-terminated string to the screen.
 * @param str The string to print.
 */
void std_print(const char* format, ...);

#endif // STD_H_