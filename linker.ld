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
}
