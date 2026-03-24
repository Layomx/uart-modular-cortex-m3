/*
 * Interfa publica para driver UART
 * */

#ifndef UART_H
#define UART_H

// Direccion base del UART0 (mps2-an36)
#define UART0_BASE 0x40004000UL

// Inicializador del UART0
void uart_init(void);

// Envio de un caracter
void uart_putc(char c);

// Envio de una cadena de caracteres
void uart_puts(const char *s);

// Envio de un numero entero en decimal
void uart_putint(int n);

// Envio de numero en hexadecimal con prefijo 0x
void uart_puthex(unsigned int n);

#endif
