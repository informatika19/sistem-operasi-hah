bcc -ansi -c -o ./bin/ext/shell.o ./src/ext/shell.c

ld86 -o ./bin/ext/shell_linked -d ./bin/ext/shell.o ./bin/ext/lib.o ./bin/lib/fileio.o ./bin/lib/folderio.o ./bin/lib/math.o ./bin/lib/param.o ./bin/lib/text.o

./extloader.out ./bin/ext/shell_linked shell