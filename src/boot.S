.section ".text.boot"
.global _start

_start:
    msr     daifset, #0xf

    mrs     x0, mpidr_el1
    and     x0, x0, #0xff
    cbz     x0, primary

secondary:
    wfe
    b       secondary

primary:
    ldr     x0, =_stack_top
    mov     sp, x0

    ldr     x0, =__bss_start
    ldr     x1, =__bss_end
    mov     x2, #0

clear_bss:
    cmp     x0, x1
    b.hs    bss_done

    str     x2, [x0], #8
    b       clear_bss

bss_done:
    bl      kernel_main

hang:
    wfe
    b       hang