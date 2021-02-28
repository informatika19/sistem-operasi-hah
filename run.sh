#!/bin/bash
./image-creator.sh > /dev/null 2>&1

# Modify here if there is new kernel file
KERNEL_FILE=("kernel" "math")
for f in ${KERNEL_FILE[@]}; do
    bcc -ansi -c -o ./bin/kernel/$f.o ./src/kernel/$f.c
done

nasm -f as86 ./assembly/kernel.asm -o ./bin/kernel_asm.o

files="./bin/kernel_asm.o"
for f in ${KERNEL_FILE[@]}; do
    files=" $files ./bin/kernel/$f.o"
done

ld86 -o ./bin/kernel.img -d $files
dd if=./bin/kernel.img of=system.img bs=512 conv=notrunc seek=1

bochs -f if2230.config