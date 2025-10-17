# Compiler and Linker
AS = nasm
CC = gcc
LD = ld

# Directories
SRCDIR := src
INCDIR := include
BOOTDIR := boot
BUILDDIR := build

# --- 1. Find source files and include directories recursively ---
# Find all .c files in SRCDIR, and add kernel.c from the project root
C_SOURCES := $(shell find $(SRCDIR) -name '*.c') kernel.c
# Find all .s assembly files in BOOTDIR
ASM_SOURCES := $(shell find $(BOOTDIR) -name '*.s')
# Find all subdirectories in INCDIR and create compiler flags for them (e.g., -Iinclude/drivers)
INC_FLAGS := $(addprefix -I,$(shell find $(INCDIR) -type d))

# --- 2. Map source files to object files, preserving directory structure ---
# For C files from SRCDIR, map src/path/file.c to build/path/file.o
C_OBJS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(filter $(SRCDIR)/%,$(C_SOURCES)))
# Handle kernel.c (or any other .c file in the root) separately
C_OBJS += $(patsubst %.c,$(BUILDDIR)/%.o,$(notdir $(filter-out $(SRCDIR)/%,$(C_SOURCES))))
# For assembly files, map boot/file.s to build/file.o
ASM_OBJS := $(patsubst $(BOOTDIR)/%.s,$(BUILDDIR)/%.o,$(ASM_SOURCES))
# Combine all object files
OBJS := $(ASM_OBJS) $(C_OBJS)

# Target binary
TARGET := $(BUILDDIR)/myos.bin

# --- 3. Update compiler and linker flags ---
ASFLAGS := -f elf32
# Added $(INC_FLAGS) to CFLAGS to include all subdirectories
CFLAGS := -m32 -I$(INCDIR) -ffreestanding -O2 -Wall -Wextra -c $(INC_FLAGS)
LDFLAGS := -m elf_i386 -T linker.ld

# Phony targets prevent conflicts with files of the same name
.PHONY: all run clean

# --- 4. Main build rules ---
# Default target
all: $(TARGET)

# Link the final binary using all object files as prerequisites
$(TARGET): $(OBJS)
	@echo "LD   $@"
	$(LD) $(LDFLAGS) -o $@ $^

# --- 5. Generic compilation rules ---
# This single rule handles all .c files from the src/ directory and its subdirectories.
# It automatically creates the necessary subdirectories in the build folder.
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@echo "CC   $<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Specific rule for kernel.c in the project root
$(BUILDDIR)/kernel.o: kernel.c
	@echo "CC   $<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# This rule handles all .s files from the boot/ directory and its subdirectories.
$(BUILDDIR)/%.o: $(BOOTDIR)/%.s
	@echo "AS   $<"
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

# --- 6. Utility rules ---
# Rule to run the OS in QEMU
# Added 'all' as a dependency to ensure the project is built before running
run: all
	qemu-system-i386 -curses -kernel $(TARGET) -smp 4 -m 512M

# Clean build artifacts
clean:
	@echo "Cleaning..."
	rm -rf $(BUILDDIR)/*