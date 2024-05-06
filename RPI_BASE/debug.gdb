define hook-stop
x/20i $pc
info registers x0
info registers x1
info registers x2
info registers x3
info registers x4
info registers x5
info registers x6
info registers pc
info registers sp
x/40xw $sp
end
add-symbol-file ./build/kernel.elf 0x80000
target remote localhost:4269
b main
c
