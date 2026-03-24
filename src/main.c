/*
 * Programa principal 
 * */

#include "../drivers/uart.h"

static void demo_tipos(void) {
    uart_puts("Tipos de datos:\n");

    uart_puts("Decimal: ");
    uart_putint(42);
    uart_puts("\r\n");

    uart_puts("Negativo: ");
    uart_putint(-42);
    uart_puts("\r\n");

    uart_puts("Hexadecimal: ");
    uart_puthex(0xDEADBEEF);
    uart_puts("\r\n");

    uart_puts("Registro: ");
    uart_puthex(UART0_BASE); // DEBUG: Mostrar la dirección base del UART
    uart_puts("\r\n");
}

int main(void) {
    uart_init();
    
    uart_puts("\r\nFirmware iniciado");
    uart_puts("Cortex-M3 corriendo en QEMU\r\n");

    demo_tipos();

    uart_puts("Bucle principal\r\n");

    int tick = 0;

    while (1) {
        uart_puts("tick #");
        uart_putint(tick);
        uart_puts("\r\n");
        tick++;

        volatile int i;
        for (i = 0; i < 1000000; i++); // Delay por software NOTE: realmente se usa SysTick
    }

    return 0;
}
