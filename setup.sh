./image-creator.sh

./kernel.sh

#nasm -f as86 ./assembly/lib.asm -o ./bin/lib.o
#bcc -ansi -c -o ./bin/shell/shell.o ./src/shell/shell.c
#ld86 -o ./shell -d ./bin/shell/shell.o ./bin/lib.o

cd src/loader/extloader
go build -o ../../../extloader.out
cd ../../../


cd src/loader/folderloader
go build -o ../../../folderloader.out
cd ../../../

./ext.sh
./test.sh