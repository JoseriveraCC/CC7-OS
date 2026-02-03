#include "stdio.h"
#include "os.h"
#include <stdarg.h>

void PRINT(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[64];

    for (const char *p = format; *p != '\0'; p++) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 's': uart_puts(va_arg(args, char*)); break;
                case 'd': 
                    uart_itoa(va_arg(args, int), buffer);
                    uart_puts(buffer);
                    break;
                case 'f':
                    uart_ftoa((float)va_arg(args, double), buffer, 2);
                    uart_puts(buffer);
                    break;
                case 'c': uart_putc((char)va_arg(args, int)); break;
            }
        } else {
            uart_putc(*p);
        }
    }
    va_end(args);
}

void READ(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[64];
    uart_gets_input(buffer, 64);

    for (const char *p = format; *p != '\0'; p++) {
        if (*p == '%') {
            p++;
            if (*p == 'd') *(va_arg(args, int*)) = uart_atoi(buffer);
            else if (*p == 'f') *(va_arg(args, float*)) = uart_atof(buffer);
            else if (*p == 's') { }
        }
    }
    va_end(args);
}