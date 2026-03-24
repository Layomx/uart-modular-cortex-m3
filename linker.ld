/*
 * Puedo decir con bastante fe que ya entiendo mejor lo que es linker script, aqui le decimos al enlazador como organizar el binario en la memoria del microcontrolador, como organizar los vectores de interrupciones, donde va el codigo, etc. Sin embargo, aun tengo muchisimo que mejorar y aprender sobre esto.
 *
 * Segun la maquina mps2-an35 en QEMU se tiene el siguiente mapa de memoria:
 * flash en 0x00000000 donde vivira el codigo
 * ram en 0x20000000 donde viviran las variables
 * de forma muy simplificada
 * */

MEMORY {
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

ENTRY(Reset_Handler)

/* Este simbolo se utiliza en startup.c es el final de la ram y ahi empieza el stack ARM */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS {
    .text : {
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
    } > FLASH

    .data : {
        _sdata = .;
        *(.data*)
        _edata = .;
    } > RAM AT > FLASH

    .bss : {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > RAM
}
