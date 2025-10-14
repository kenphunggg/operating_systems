# Compiler and Linker
AS = nasm
CC = gcc
LD = ld

# Directories
SRCDIR := src
INCDIR := include
BOOTDIR := boot
BUILDDIR := build

# Source files
C_SOURCES := $(wildcard $(SRCDIR)/*.c) kernel.c
ASM_SOURCES := $(wildcard $(BOOTDIR)/*.s)

# Object files
C_OBJS := $(patsubst %.c, $(BUILDDIR)/%.o, $(notdir $(C_SOURCES)))
ASM_OBJS := $(patsubst $(BOOTDIR)/%.s, $(BUILDDIR)/%.o, $(ASM_SOURCES))
OBJS := $(ASM_OBJS) $(C_OBJS)

# Target binary
TARGET := $(BUILDDIR)/myos.bin

# Flags
ASFLAGS := -f elf32
CFLAGS := -m32 -I$(INCDIR) -ffreestanding -O2 -Wall -Wextra -c
LDFLAGS := -m elf_i386 -T linker.ld

# Default target
all: $(BUILDDIR) $(TARGET)

# Link the final binary
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJS)

# Build directory
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Compile kernel.c (in project root)
$(BUILDDIR)/kernel.o: kernel.c
	$(CC) $(CFLAGS) $< -o $@

# Compile C files in src/
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

# Compile .s files in boot/
$(BUILDDIR)/%.o: $(BOOTDIR)/%.s
	$(AS) $(ASFLAGS) $< -o $@

# Rule to run the OS in QEMU
# Tag *-smp 4* emulate 4 CPU core
# Tag *-m 512M* emulate 512MB of RAM
run: $(TARGET)
	qemu-system-i386 -curses -kernel $(TARGET) -smp 4 -m 512M

# Clean build artifacts
clean:
	rm -f $(BUILDDIR)/*
