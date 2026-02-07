#include "os.h"

volatile unsigned int * const UART0 = (unsigned int *)UART0_BASE;

void uart_putc(char c) {
    while (UART0[UART_FR / 4] & UART_FR_TXFF);
    UART0[UART_DR / 4] = c;
}

char uart_getc() {
    while (UART0[UART_FR / 4] & UART_FR_RXFE);
    return (char)(UART0[UART_DR / 4] & 0xFF);
}

void uart_puts(const char *s) {
    while (*s) uart_putc(*s++);
}

void uart_gets_input(char *buffer, int max_length) {
    int i = 0;
    char c;
    while (i < max_length - 1) {
        c = uart_getc();
        if (c == '\n' || c == '\r') {
            uart_putc('\n');
            break;
        }
        uart_putc(c);
        buffer[i++] = c;
    }
    buffer[i] = '\0';
}

// Conversión de entero a string
void uart_itoa(int num, char *buffer) {
    int i = 0;
    int is_negative = 0;
    if (num == 0) { buffer[i++] = '0'; buffer[i] = '\0'; return; }
    if (num < 0) { is_negative = 1; num = -num; }
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    if (is_negative) buffer[i++] = '-';
    buffer[i] = '\0';
    // Invertir string
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
}

int uart_atoi(const char *s) {
    int num = 0, sign = 1, i = 0;
    if (s[0] == '-') { sign = -1; i++; }
    for (; s[i] >= '0' && s[i] <= '9'; i++) num = num * 10 + (s[i] - '0');
    return sign * num;
}

// Implementación de punto flotante para el laboratorio
float uart_atof(const char *s) {
    float res = 0.0, factor = 1.0;
    int i = 0, dot_found = 0;

    if (s[i] == '-') { 
        factor = -1.0; i++; 
    }
    //uart_putc(s[i]);
    for (; s[i]; i++) {
        // Encuentra el punto
        if (s[i] == '.') { 
            dot_found = 1; continue; 
        }

        // Convierte ascii a al valor del numero
        int digit = s[i] - '0';
        if (digit >= 0 && digit <= 9) {
            if (!dot_found) {
                res = res * 10.0 + digit;
            } else { 
                factor /= 10.0; 
                res += digit * factor; 
            }
        }
    }
    return res;
}

void uart_ftoa(float n, char *res, int afterpoint) {
    // Manejo de signo para el número completo
    if (n < 0) {
        *res++ = '-';
        n = -n;
    }

    int parteInt = (int)n;
    float parteFloat = n - (float)parteInt;

    // Convertir parte entera
    uart_itoa(parteInt, res);

    // Mover el puntero al final de la parte entera
    int i = 0;
    while (res[i] != '\0') i++;

    if (afterpoint != 0) {
        res[i++] = '.';

        // Calcular el factor de escala
        int scale = 1;
        for (int j = 0; j < afterpoint; j++) {
            scale *= 10;
        }

        // convertir a entero y 0.5f para redondear
        int f_int = (int)(parteFloat * scale + 0.5f);

        // 0s a la izquierda
        int temp_scale = scale / 10;
        while (temp_scale > 0) {
            if (f_int < temp_scale) {
                res[i++] = '0';
                temp_scale /= 10;
            } else {
                break; 
            }
        }

        // Convertir el valor numérico restante
        if (f_int != 0) {
            uart_itoa(f_int, res + i);
        } else {
            res[i] = '\0';
        }
    }
}