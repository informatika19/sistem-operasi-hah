mkdir bin bin/kernel bin/shell bin/ext bin/lib > /dev/null 2>&1
cp default-map.img bin/map.img > /dev/null 2>&1
dd if=/dev/zero of=./bin/files.img bs=512 count=2 > /dev/null 2>&1
dd if=/dev/zero of=./bin/sectors.img bs=512 count=1 > /dev/null 2>&1

dd if=/dev/zero of=system.img bs=512 count=2880 > /dev/null 2>&1
nasm ./assembly/bootloader.asm -o ./bin/bootloader.img
dd if=./bin/bootloader.img of=system.img bs=512 count=1 conv=notrunc > /dev/null 2>&1

# Uncomment to create new map, files, and sectors image
# dd if=/dev/zero of=./bin/map.img bs=512 count=1
# dd if=/dev/zero of=./bin/files.img bs=512 count=2
# dd if=/dev/zero of=./bin/sectors.img bs=512 count=1
# Dont forget to hex edit the map.img

dd if=./bin/map.img of=system.img bs=512 count=1 seek=256 conv=notrunc > /dev/null 2>&1
dd if=./bin/files.img of=system.img bs=512 count=2 seek=257 conv=notrunc > /dev/null 2>&1
dd if=./bin/sectors.img of=system.img bs=512 count=1 seek=259 conv=notrunc > /dev/null 2>&1
