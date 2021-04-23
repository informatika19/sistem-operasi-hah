# Script List Usage

1. clean.sh : Menghapus semua image dan binary files
2. setup.sh : Memanggil image-creator.sh untuk membersihkan image dan membuat image baru lalu mengompile kernel dan file external loader
3. runBosch.sh : Menjalankan Bosch

# Milestone 1

Pada sistem operasi ini tersedia beberapa file yaitu sebagai berikut.
- Kernel.asm: mengandung fungsi putInMemory, interrupt, makeInterrupt, dan menyediakan service InterruptRoutine
- Kernel.c: Tempat implementasi fungsi yang diminta menggunakan interrupt dari kernel.asm
- Bootloader.asm

## Cara Kerja Interrupt
Interrupt adalah suatu proses yang dilakukan oleh komputer untuk mengirimkan sinyal. Interrupt bersifat asynchronous. Interrupt merupakan sebuah cara untuk hardware dan software berkomunikasi. Komputer akan menunggu interrupt dari hardware dan kembali lagi ke program utama secara terus menerus. Interrupt digunakan untuk mengimplementasikan fungsi-fungsi syscall seperti input/ouput ke layar dalam bentuk text dan gambar. Tidak hanya itu, banyak fungsi lainnya dalam interrupt.

Fungsi interrupt pada OS ini terdiri dari 5 integer. Integer pertama digunakan untuk menyatakan tujuan service hardware komputer dan 4 integer sisanya berfungsi sebagai parameter/argument yang akan digunakan sesuai kebutuhan.

Sistem operasi sudah mampu membaca input ketikan dari keyboard pengguna melalui BIOS interrupt 0x16 dan mencetak tulisan ke layar melalui BIOS interrupt 0x10. BIOS interrupt calls tersebut akan memanggil Input/Output System(BIOS) software pada computer agar dapat menjalankan fungsi I/O yang diminta oleh program, mengembalikan informasi terkait sistem, maupun melakukan kontrol langsung pada hardware.

## Cara Kerja Kode di kernel.asm
File kernel.asm adalah kode assembly dari kernel. File kernel.asm mengandung fungsi puntInMemory, interrupt, makeInterrupt, dan menyediakan service InterruptRoutine. File kernel.c akan menggunakan fungsi-fungsi ini di dalam implementasinya. File kernel.asm akan dikompilasi oleh nasm untuk membentuk object code kernel_asm.o yang kemudian akan digabungkan dengan object code dari hasil kompilasi kernel.c melalui proses linking.

# Dokumentasi Command Utilitas

- ls - list directory content
- cp [src] [dest] - copy file/folder to folder
- mv [src] [dest] - move file/folder to folder
- mkdir [name] - make new folder
- rm [name] - remove file/folder
- cat [name] - print file contents
- ln [src] [dest] - create symbolic link, khusus ln, src dest hanya dalam 1 folder, tidak bisa absolute path.