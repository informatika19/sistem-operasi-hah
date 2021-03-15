nasm -f as86 ./assembly/lib.asm -o ./bin/assembly/lib.o

bcc -ansi -c -o ./bin/shell/shell.o ./src/shell/shell.c

ld86 -o ./bin/shell/linkedShell.o -d ./bin/shell/shell.o ./bin/assembly/lib.o