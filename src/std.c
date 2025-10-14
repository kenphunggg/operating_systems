#include "define.h"

/*
 * Scrolls the screen up by one line.
 */
void scroll()
{
    unsigned short *vga_buffer = (unsigned short *)VGA_ADDRESS;
    // Move every line up by one.
    for (int y = 0; y < VGA_HEIGHT - 1; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            const int current_index = y * VGA_WIDTH + x;
            const int next_index = (y + 1) * VGA_WIDTH + x;
            vga_buffer[current_index] = vga_buffer[next_index];
        }
    }
    // Clear the last line.
    for (int x = 0; x < VGA_WIDTH; x++)
    {
        const int index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        vga_buffer[index] = ' ' | (0x07 << 8);
    }
    CURSOR_Y = VGA_HEIGHT - 1;
}

/*
 * Puts a single character on the screen at the current cursor position.
 * Handles newlines and scrolling.
 * @param c The character to print.
 */
void putchar(char c)
{
    unsigned short *vga_buffer = (unsigned short *)VGA_ADDRESS;

    if (c == '\n')
    {
        CURSOR_X = 0;
        CURSOR_Y++;
    }
    else
    {
        const int index = CURSOR_Y * VGA_WIDTH + CURSOR_X;
        // The attribute byte is 0x07 for light grey on black.
        vga_buffer[index] = c | (0x07 << 8);
        CURSOR_X++;
    }

    if (CURSOR_X >= VGA_WIDTH)
    {
        CURSOR_X = 0;
        CURSOR_Y++;
    }

    if (CURSOR_Y >= VGA_HEIGHT)
    {
        scroll();
    }
}

/*
 * Clears the screen by filling it with spaces.
 */
void std_clear_screen()
{
    unsigned short *vga_buffer = (unsigned short *)VGA_ADDRESS;
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            const int index = y * VGA_WIDTH + x;
            // Set character to space with a light grey on black color attribute.
            vga_buffer[index] = ' ' | (0x07 << 8);
        }
    }
    CURSOR_X = 0;
    CURSOR_Y = 0;
}

/*
 * Prints a null-terminated string to the screen.
 * @param str The string to print.
 */
void std_print(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        putchar(str[i]);
    }
}

void itoa(int n, char str[])
{
    int i = 0;
    int is_negative = 0;

    // Handle 0 explicitly
    if (n == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers
    if (n < 0)
    {
        is_negative = 1;
        n = -n;
    }

    // Process individual digits
    while (n != 0)
    {
        int rem = n % 10;
        str[i++] = rem + '0';
        n = n / 10;
    }

    // Add sign if needed
    if (is_negative)
    {
        str[i++] = '-';
    }

    str[i] = '\0'; // Null-terminate the string

    // Reverse the string
    for (int j = 0, k = i - 1; j < k; j++, k--)
    {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}
