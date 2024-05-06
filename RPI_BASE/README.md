# Raspi Zero update

This documents updating `03_uart1` to support `raspi0` in `QEMU 5.2.50`. Several things need to change here, but fortunately `mmio` addresses are the same relative to their base address. 

### Changes:
+ raspi0 mmio base: `0x20000000` from `0x3F000000`
+ raspi0 uses ARMv6 32bit Single core cpu, so no need to pause other cores
+ since 32bit, `start.S` needed updates, registers need changing, intructions need downgrading, etc.
+ `kernel.img` is the expected name of image, start address is `0x10000` from `0x80000`

This will be usable for a base to include other C files, but Makefile may need updates.