# Milestone 1

Pada sistem operasi ini tersedia beberapa file yaitu sebagai berikut.
- Kernel.asm: mengandung fungsi putInMemory, interrupt, makeInterrupt, dan menyediakan service InterruptRoutine
- Kernel.c: Tempat implementasi fungsi yang diminta menggunakan interrupt dari kernel.asm
- Bootloader.asm

## Cara Kerja Interrupt
Interrupt adalah suatu proses yang dilakukan oleh komputer untuk mengirimkan sinyal. Interrupt bersifat asynchronous. Interrupt merupakan sebuah cara untuk hardware dan software berkomunikasi. Sistem operasi ini menggunakan interrups 0x21 untuk melakukan syscall. Sistem operasi sudah mampu membaca input ketikan dari keyboard pengguna melalui BIOS interrupt 0x16 dan mencetak tulisan ke layar melalui BIOS interrupt 0x10. BIOS interrupt calls tersebut akan memanggil Input/Output System(BIOS) software pada computer agar dapat menjalankan fungsi I/O yang diminta oleh program, mengembalikan informasi terkait sistem, maupun melakukan kontrol langsung pada hardware.

## Cara Kerja Kode di kernel.asm
File kernel.asm adalah kode assembly dari kernel. File kernel.asm mengandung fungsi puntInMemory, interrupt, makeInterrupt, dan menyediakan service InterruptRoutine. File kernel.c akan menggunakan fungsi-fungsi ini di dalam implementasinya. File kernel.asm akan dikompilasi oleh nasm untuk membentuk object code kernel_asm.o yang kemudian akan digabungkan dengan object code dari hasil kompilasi kernel.c melalui proses linking.