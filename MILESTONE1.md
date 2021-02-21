# Milestone 1

## Cara Kerja Interrupt

Pada sistem operasi ini tersedia beberapa file yaitu sebagai berikut.
- Kernel.asm: mengandung fungsi interrupt, makeInterrupt, dan menyediakan service InterruptRoutine
- Kernel.c: Tempat implementasi fungsi yang diminta menggunakan interrupt dari kernel.asm
- Bootloader.asm

Interrupt adalah suatu proses yang dilakukan oleh komputer untuk mengirimkan sinyal. Interrupt bersifat asynchronous. Interrupt merupakan sebuah cara untuk hardware dan software berkomunikasi. 