.section ".text.boot"

.global _start

_start:
    // set stack under our code
    ldr     r1, =_start
    mov     sp, r1

    // clear bss
    ldr     r1, =__bss_start
    ldr     r2, =__bss_size
1:  cmp     r2, #0
    beq     2f
    str     r0, [r1], #8
    sub     r2, r2, #1
    cmp     r2, #0
    bne     1b

    // jump to C code, should not return
2:  bl      main
    // for failsafe, halt this core too
3:  nop
    b       3b
