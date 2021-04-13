bcc -ansi -c -o ./bin/ext/test.o ./src/ext/test.c

ld86 -o ./bin/ext/test_linked -d ./bin/ext/test.o ./bin/ext/lib.o ./bin/lib/fileio.o ./bin/lib/folderio.o ./bin/lib/math.o ./bin/lib/param.o ./bin/lib/text.o

./extloader.out ./bin/ext/test_linked test