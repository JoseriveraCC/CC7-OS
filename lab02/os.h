#ifndef OS_H
#define OS_H

#include <stddef.h>

// Definiciones de hardware del UART
#define UART0_BASE 0x101f1000
#define UART_DR      0x00
#define UART_FR      0x18
#define UART_FR_TXFF 0x20
#define UART_FR_RXFE 0x10

// Prototipos de funciones de comunicación [cite: 59, 60]
void uart_putc(char c);
char uart_getc();
void uart_puts(const char *s);
void uart_gets_input(char *buffer, int max_length);

// Funciones de conversión [cite: 60, 75]
int uart_atoi(const char *s);
void uart_itoa(int num, char *buffer);
float uart_atof(const char *s);
void uart_ftoa(float n, char *res, int afterpoint);

#endif