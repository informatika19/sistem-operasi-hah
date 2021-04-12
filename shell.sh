bcc -ansi -c -o ./bin/ext/shell.o ./src/ext/shell.c

ld86 -o ./bin/ext/shell_linked -d ./bin/ext/shell.o ./bin/ext/lib.o

./extloader.out ./bin/ext/shell_linked shell