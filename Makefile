# Compiler and Linker
AS = nasm
CC = gcc
LD = ld

# Directories
SRCDIR := src
INCDIR := include
BUILDDIR := build

# Find all source files
C_SOURCES := $(wildcard $(SRCDIR)/*.c) kernel.c
ASM_SOURCES := boot.s

# Create object file names by replacing .c/.s with .o and putting them in BUILDDIR
C_OBJS := $(patsubst %.c, $(BUILDDIR)/%.o, $(notdir $(C_SOURCES)))
ASM_OBJS := $(patsubst %.s, $(BUILDDIR)/%.o, $(notdir $(ASM_SOURCES)))
OBJS := $(ASM_OBJS) $(C_OBJS)

# Target binary, now located in the build directory
TARGET := $(BUILDDIR)/myos.bin

# Compiler and Linker Flags
ASFLAGS := -f elf32
# Add -I$(INCDIR) to tell GCC where to find header files
CFLAGS := -m32 -I$(INCDIR) -ffreestanding -O2 -Wall -Wextra -c
LDFLAGS := -m elf_i386 -T linker.ld

# Default rule
all: $(BUILDDIR) $(TARGET)

# Rule to build the final OS image
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJS)

# Rule to create the build directory
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Rule to compile C files from the root directory (kernel.c)
$(BUILDDIR)/kernel.o: kernel.c
	$(CC) $(CFLAGS) $< -o $@

# Rule to compile C files from the src/ directory
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

# Rule to compile assembly files
$(BUILDDIR)/%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

# Rule to run the OS in QEMU
# Tag *-smp 4* emulate 4 CPU core
# Tag *-m 512M* emulate 512MB of RAM
run: $(TARGET)
	qemu-system-i386 -curses -kernel $(TARGET) -smp 4 -m 512M

# Rule to clean up build files
# Now we only need to remove the build directory.
clean:
	rm -rf $(BUILDDIR)

