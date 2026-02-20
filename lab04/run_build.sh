#!/bin/bash

# Imprimir un mensaje en la consola
echo "Iniciando la compilación del Laboratorio 004..."

# Compilar cada archivo con la bandera -Wall para ver todas las advertencias
gcc -Wall tarea1.c -o tarea1
gcc -Wall tarea2.c -o tarea2
gcc -Wall tarea3.c -o tarea3
gcc -Wall tarea4.c -o tarea4
gcc -Wall tarea5.c -o tarea5

echo "¡Compilación exitosa!"
echo "Archivos generados: tarea1, tarea2, tarea3, tarea4, tarea5"
echo "Para ejecutar uno, escribe por ejemplo: ./tarea3"