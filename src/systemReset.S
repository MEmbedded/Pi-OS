.equ PM_RSTC,  0x3F10001C
.equ PM_WDOG,  0x3F100024

.equ PM_PASSWORD,       0x5A000000
.equ PM_RSTC_FULL_RESET, 0x00000020

.global system_restart

system_restart:
    msr daifset, #0xf

    ldr x0, =PM_WDOG
    ldr w1, =(PM_PASSWORD | 1)
    str w1, [x0]

    ldr x0, =PM_RSTC
    ldr w1, =(PM_PASSWORD | PM_RSTC_FULL_RESET)
    str w1, [x0]

1:
    wfe
    b 1b