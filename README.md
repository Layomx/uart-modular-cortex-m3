# UART Modular — ARM Cortex-M3 Bare Metal

Driver UART bare metal para ARM Cortex-M3 corriendo en QEMU, escrito completamente en C sin sistema operativo ni librerías externas.

## Descripción

Implementación desde cero de un driver UART con acceso directo a registros de hardware mediante punteros a direcciones de memoria. El proyecto incluye startup code, linker script personalizado y tabla de vectores de interrupciones. Este es de mis proyectos de aprendizaje, es parecido a uno anterior pero tiene algunos conceptos nuevos que he retocado debido a que he leido nueva documentacion y queria hacer pruebas, es tambien, una gran prueba de depuracion con gdb. 

## Hardware (simulado)

- **Procesador:** ARM Cortex-M3
- **Plataforma:** QEMU mps2-an385
- **Memoria:** 256KB FLASH / 64KB RAM

## Dependencias
```bash
sudo apt install gcc-arm-none-eabi qemu-system-arm make
```

## Compilacion y ejecucion
```bash
# Compilar
make

# Ejecutar en QEMU
make run

# Depurar con GDB
make debug
```

En otra terminal para depurar:
```bash
gdb-multiarch build/firmware.elf
(gdb) target remote :3333
(gdb) break main
(gdb) continue
... demas comandos
```

Como salir de QEMU: `Ctrl+A` seguido de `X`

## Funcionalidades del driver UART

- `uart_init():` inicializa el transmisor UART
- `uart_putc(char c):` transmite un carácter
- `uart_puts(const char *s):` transmite una cadena
- `uart_putint(int n):` transmite un entero en decimal
- `uart_puthex(unsigned int n):` transmite un entero en hexadecimal

## Salida esperada
```
Firmware iniciado
Cortex-M3 corriendo en QEMU
Tipos de datos:
Decimal: 42
Negativo: -42
Hexadecimal: 0xDEADBEEF
Registro: 0x40004000
Bucle principal
tick #0
tick #1
un bucle infinito hasta cerrarse qemu
```

## Cosas que mas o menos aprendi y he entendido

- Linker scripts y organización de memoria en ARM
- Tabla de vectores de interrupciones
- Acceso directo a registros de hardware mediante punteros
- Startup code bare metal
- Cross-compilation con arm-none-eabi-gcc
- Depuración remota con GDB y QEMU
