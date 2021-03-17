dd if=/dev/zero of=system.img bs=512 count=2880
nasm ./assembly/bootloader.asm -o ./bin/bootloader.img
dd if=./bin/bootloader.img of=system.img bs=512 count=1 conv=notrunc

dd if=./bin/map.img of=system.img bs=512 count=1 seek=256 conv=notrunc
dd if=./bin/files.img of=system.img bs=512 count=2 seek=257 conv=notrunc
dd if=./bin/sectors.img of=system.img bs=512 count=1 seek=259 conv=notrunc

./kernel.sh

nasm -f as86 ./assembly/lib.asm -o ./bin/lib.o
bcc -ansi -c -o ./bin/shell/shell.o ./src/shell/shell.c
ld86 -o ./shell -d ./bin/shell/shell.o ./bin/lib.o

cd src/loader
go build -o ../../extloader.out
cd ../..
./loadfile shell shell