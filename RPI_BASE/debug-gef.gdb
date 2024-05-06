gef config gef.debug True
set architecture aarch64
add-symbol-file build/kernel.elf 0x80000
gef-remote --qemu-mode localhost:4269
