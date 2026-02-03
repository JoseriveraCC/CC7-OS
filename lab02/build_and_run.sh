#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Remove previous compiled objects and binaries
echo "Limpiando archivos de compilación anteriores..."
rm -f *.o calculadora.elf calculadora.bin

echo "Assembling startup.s..."
arm-none-eabi-as -o root.o root.s

echo "Compilando os.c..."
arm-none-eabi-gcc -c -o os.o os.c

echo "Compilando librerías de lenguaje..."
arm-none-eabi-gcc -c -o stdio.o stdio.c
arm-none-eabi-gcc -c -o string.o string.c

echo "Compiling main.c..."
arm-none-eabi-gcc -c -o main.o main.c

echo "Linking object files..."
#arm-none-eabi-ld -T linker.ld -o calculadora.elf root.o os.o stdio.o string.o main.o
arm-none-eabi-gcc -nostartfiles -T linker.ld -o calculadora.elf root.o os.o stdio.o string.o main.o -lgcc

echo "Converting ELF to binary..."
arm-none-eabi-objcopy -O binary calculadora.elf calculadora.bin

echo "Running QEMU..."
qemu-system-arm -M versatilepb -nographic -audio none -kernel calculadora.elf
