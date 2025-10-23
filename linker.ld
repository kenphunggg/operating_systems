ENTRY(_start)

SECTIONS {
    /* The kernel will be loaded at 1MB. */
    . = 1M;

    .text : {
        *(.multiboot)
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

    _end = .; /* Symbol for the end of the kernel binary */
}
