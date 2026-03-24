/*
 * Implementacion de Driver UART con acceso directo a registros de hardware mediante punteros a direcciones fijas de memoria.
 */

#include "uart.h"

// Definición de las direcciones base para los registros del UART
#define UART_DATA (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UART_STATE (*(volatile unsigned int *)(UART0_BASE + 0x04))
#define UART_CTRL (*(volatile unsigned int *)(UART0_BASE + 0x08))

#define UART_TX_FULL (1 << 0) // Bit 0: TX buffer lleno 
#define UART_TX_EN (1 << 0)   // Bit 0: Habilitar transmisión

void uart_init() {
    // Habilita el transmisor UART escribiendo en el registro de control
    // TODO: Lei que en el bit 0 del registro de control se habilita el transmisor, pero tambien se puede habilitar el receptor y configurarse el baud rate, etc. Deberia revisar a fondo la documentacion del hardware para configurar completamente el UART cuando pueda
    UART_CTRL = UART_TX_EN;
}

void uart_putc(char c) {
    // Espera hasta que el buffer de transmisión no esté lleno: El & verifica si el bit TX_FULL esta en 1, cuando el UART esta listo el bit baja a 0 y sale del bucle
    while (UART_STATE & UART_TX_FULL);

    UART_DATA = (unsigned int)c; // Escribe el carácter en el registro de datos (UART_DATA) para transmitirlo
}

void uart_puts(const char *s) {
    // Itera sobre el string hasta encontrar le caracter nulo y transmite cada caracter usando uart_putc
    while (*s != '\0'){
        uart_putc(*s); 
        s++;
    }
}

void uart_putint(int n) {
    if (n == 0) {
        uart_putc('0');
        return;
    }

    if (n < 0) {
        uart_putc('-');
        n = -n; // Convierte el número a positivo para facilitar la conversión
    }

    char buffer[12];
    int i = 0;

    while (n > 0) {
        buffer[i++] = '0' + (n % 10); // Obtiene el digito menos significa y lo convierte a caracter
        n /= 10; // Elimina el digito menos significativo
    }

    // Envio al buffer al reves
    while (i > 0) {
        uart_putc(buffer[--i]); // Envia el caracter al UART
    } 
}

void uart_puthex(unsigned int n) {
    // Tabla de digitos hexadecimales
    const char hex_digits[] = "0123456789ABCDEF";

    uart_puts("0x"); // Prefijo para indicar que es un número hexadecimal
    
    int i;
    for (i = 28; i >= 0; i -= 4) {
        uart_putc(hex_digits[(n >> i) & 0xF]);
    }
};
