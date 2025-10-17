#ifndef DEFINE_H_
#define DEFINE_H_

// Define the address of the VGA video memory.
#define VGA_ADDRESS 0xB8000

// Define VGA text mode dimensions.
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Keep track of the current cursor position.
extern int CURSOR_X;
extern int CURSOR_Y;

#define NULL ((void*)0)

#endif // DEFINE_H_