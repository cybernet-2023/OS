#!/bin/bash
i386-elf-gcc -m32 -ffreestanding -c src/kernel.c -o krnl.o
i386-elf-gcc -m32 -ffreestanding -c src/stdio.c -o stdio.o
i386-elf-gcc -m32 -ffreestanding -c src/string.c -o string.o
nasm -f elf32 src/entry.asm -o entry.o
i386-elf-ld -m elf_i386 -T link.ld -o kernel entry.o krnl.o stdio.o string.o