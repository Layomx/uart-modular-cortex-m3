/*
 * Arranque del microcontrolador, similar al que escribi en otro momento
 * */

extern unsigned long _estack; 
extern int main(void);

// Reset_Handler es una funcion ejecutada por el microcontrolador.
// TODO: Aqui normalmente se copia la seccion .data de flash a ram y .bss se pone en cero, segun he leido, en el futuro quisiera implementarlo, de momento solo llamo a main directamente para hacerlo simple y no complicarme con cosas que no entiendo del todo, aunque es algo que quiero aprender a hacer en el futuro.
void Reset_Handler(void) {
    main();
    while(1);
}

// La tabla de vectores es un array de punteros a funciones, ahora la entiendo mejor, ARM espera encontrarla al inicio de la memoria flash (0x00000000). La seccion .isr_vector garantizara esto.
// Posicion 0: El valor inicial del stack pointer, que es la direccion de memoria donde se encuentra el final de la pila, en este caso el valor de _estack.
// Posicion 1: La direccion de memoria de la funcion Reset_Handler, que es la funcion que se ejecutara al resetear el microcontrolador.
// TODO: Las demas posiciones son handlers de excepciones que dejare en 0 por ahora porque no las conozco y  0 es un valor valido para un puntero a funcion, aunque en el futuro quiero aprender a escribir handlers para las excepciones mas comunes, como HardFault_Handler, etc.
__attribute__ ((section(".isr_vector")))
void (*vector_table[])(void) = {
    (void (*)(void))(&_estack),  // SP inicial
    Reset_Handler, // Reset
    0, // NMI
    0, // HardFault
};
