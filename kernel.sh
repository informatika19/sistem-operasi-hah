#Modify here if there is new kernel file
KERNEL_FILE=("math" "string" "file" "folder" "sector" "kernel")
for f in ${KERNEL_FILE[@]}; do
    bcc -ansi -c -o ./bin/kernel/$f.o ./src/kernel/$f.c
done

nasm -f as86 ./assembly/kernel.asm -o ./bin/kernel_asm.o

files=""
for f in ${KERNEL_FILE[@]}; do
    files="./bin/kernel/$f.o $files"
done
files="$files ./bin/kernel_asm.o"

ld86 -o ./bin/kernel.img -d $files
dd if=./bin/kernel.img of=system.img bs=512 seek=1 conv=notrunc > /dev/null 2>&1