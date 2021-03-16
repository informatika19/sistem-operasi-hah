rm system.img > /dev/null 2>&1
rm -rf bin > /dev/null 2>&1

mkdir bin bin/kernel bin/shell
cp default-map.img bin/map.img > /dev/null 2>&1
dd if=/dev/zero of=./bin/files.img bs=512 count=2 > /dev/null 2>&1
dd if=/dev/zero of=./bin/sectors.img bs=512 count=1 > /dev/null 2>&1