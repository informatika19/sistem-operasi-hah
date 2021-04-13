#!/bin/bash

nasm -f as86 ./assembly/lib.asm -o ./bin/ext/lib.o

bcc -ansi -c -o ./bin/lib/fileio.o ./src/lib/fileio.c
bcc -ansi -c -o ./bin/lib/folderio.o ./src/lib/folderio.c
bcc -ansi -c -o ./bin/lib/math.o ./src/lib/math.c
bcc -ansi -c -o ./bin/lib/param.o ./src/lib/param.c
bcc -ansi -c -o ./bin/lib/text.o ./src/lib/text.c
bcc -ansi -c -o ./bin/lib/program.o ./src/lib/program.c
bcc -ansi -c -o ./bin/lib/sector.o ./src/lib/sector.c

bcc -ansi -c -o ./bin/ext/cat.o ./src/ext/cat.c
bcc -ansi -c -o ./bin/ext/ls.o ./src/ext/ls.c

ld86 -o ./bin/ext/cat_linked -d ./bin/ext/cat.o ./bin/ext/lib.o ./bin/lib/fileio.o ./bin/lib/folderio.o ./bin/lib/math.o ./bin/lib/param.o ./bin/lib/text.o ./bin/lib/program.o ./bin/lib/sector.o
ld86 -o ./bin/ext/ls_linked -d ./bin/ext/ls.o ./bin/ext/lib.o ./bin/lib/fileio.o ./bin/lib/folderio.o ./bin/lib/math.o ./bin/lib/param.o ./bin/lib/text.o ./bin/lib/program.o ./bin/lib/sector.o

./extloader.out ./bin/ext/cat_linked cat
./extloader.out ./bin/ext/ls_linked ls