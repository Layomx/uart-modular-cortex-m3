/*
 * Public inteface for UART driver
 *
 * This file defines the public interface for a UART driver for the MPS2 architecture. The UART (Universal Asynchronous Receiver/Transmitter) is a hardware peripheral used for serial communication, allowing the microcontroller to send and receive data over a serial interface. This drivers provides interacts directly with the UART hardware registers to perform its basic functions suck as initialization, sending characters, etc. 
 *
 * In MPS2 architecture UART ports are CMSDK components, which are simple UART peripheral with a basic set of features. Which leads to use CMSDK documentation and also the mps2-an385 application note to understand the register map and how to interact with the UART hardware.
 *
 * References: 
 *   - CMSDK UART documentation: https://developer.arm.com/documentation/ddi0479/d/apb-components/apb-uart/programmers-model
 *   - MPS2-AN385 application note: https://developer.arm.com/documentation/dai0385/d/ (Avaible as downloadable PDF)
 *
 * Disclaimer: This code is heavily commented to provide a detailed explanation of the UART hardware and how to interact with it. Also to come back to this code in the future and understand the design decisions I previously made. I'd say that is also useful for anyone who randomly read this code and want to understand what did I do and why.
 * */

#ifndef UART_H
#define UART_H

#include <stdint.h>

/*
 * Base addresses for UART peripherals in MPS2 architecture:
 * The MPS2 platform typically includes multiple UART ports (UART0 to UART4), each mapped to a specific base address in the memory space. These addresses are defined in the MPS2 documentation and are used to access the corresponding UART hardware registers.
 * */
#define UART0_BASE (0x40004000UL) 
#define UART1_BASE (0x40005000UL)
#define UART2_BASE (0x40006000UL)
#define UART3_BASE (0x40007000UL)
#define UART4_BASE (0x40008000UL)

/*
 * UART Register Map (MPS2 AN385):
 *
 * These registers are memory-maped and directly driven by the UART hardware. 
 * Access must be perfomed usign volatile pointers to ensure that the compiler does not optimize away reads/writes, as these registers can change independently of the program flow.
 *
 * Register layout (offsets from UARTx_BASE):
 *
 * 0x00 - UART_DATA: 
 *   - Write: Writing to this register sends data to the UART transmitter. If the transmitter is busy, the data will be buffered until it can be sent. The write must only occur when the TX buffer is not full.
 *   - Read: Reading from this register retrieves data received by the UART receiver. The caller must check the RX buffer status before reading, otherwise the data may be invalid.
 * 0x04 UART_STATE: 
 *   - Read only register reflecting real-time status of the UART. The bits in this register indicate: 
 *   - Bit 0 (UART_STATE_TX_FULL): Indicates if the transmit buffer is full (1) or has space (0). The transmitter can only be written to when this bit is 0.
 *   - Bit 1 (UART_STATE_RX_FULL): Indicates if the receive buffer has data available (1) or is empty (0). The receiver can only be read from when this bit is 1.
 *   - Bit 2 (UART_STATE_TX_OVERRUN): Data was lost because the transmitter was written while the TX buffer was full. This bit is set to 1 to clear this flag, the software must write a 1 to this bit.
 *   - Bit 3 (UART_STATE_RX_OVERRUN): Data was lost because the receiver was not read while the RX buffer was full. This bit is set to 1 to clear this flag, the software must write a 1 to this bit. It also indicates the software is not reading the data fast enough, and the next received byte will be lost if this condition is not cleared.
 *   
 * 0x08 - UART_CTRL:
 * - Bit 0 (TX_EN): Enable (1) or disable (0) the UART transmitter. If disabled, data in UART_DATA will not be shifted out.
 * - Bit 1 (RX_EN): Enable (1) or disable (0) the UART receiver. If disabled, incoming serial data is ignored.
 * - Bit 2 (TX_INT_EN): Enable (1) transmit interrupts. Triggered when the TX buffer becomes empty.
 * - Bit 3 (RX_INT_EN): Enable (1) receive interrupts. Triggered when the RX buffer has valid data.
 * - Bit 4 (TX_OVR_INT_EN): Enable (1) transmit overrun interrupts. Triggered if a write occurs while TX_FULL is 1.
 * - Bit 5 (RX_OVR_INT_EN): Enable (1) receive overrun interrupts. Triggered if a new byte arrives before the previous was read.
 * - Bit 6 (HS_TM): High-Speed test mode. MUST be 0 for normal operation.
 *
 * 0x0C - UART_INTSTATUS: 
 * - Read-only register reflecting pending interrupts. The bits in this register indicate:
 *   - Bit 0: TX interrupt. Write 1 to clear.
 *   - Bit 1: RX interrupt. Write 1 to clear.
 *   - Bit 2: TX overrun interrupt. Write 1 to clear.
 *   - Bit 3: RX overrun interrupt. Write 1 to clear.
 *   (This literally writted in the original documentation but I have no idea how to trigger this interrupts because I haven't tested not implemented any interrupt handler yet)
 *
 * 0x10 - UART_BAUDDIV (Baud Rate Divider):
 * - Defines the baud rate of the UART. The value is calculated as: 
 * BaudDivider = SystemFrequency / BaudRate.
 * Minimum value is 16. Changes here take effect immediately.
 * (This is also is very literal writted in the documentation but I have googled a bit of this and plan to implement something with this later)
  * */

/*
 * UART_TypeDef structure: Due to MPS2 architecture and CMSDK UART design, there are 5 different UART peripherals (UARTO0 to UART4), each with the same register layour but different base adddresses. To make easy the access to these registers, I defined a structure that represents the UART register map, and then I can access to each UART peripheral registers without repeating the same code for each one. 
 *
 * C structures ensure that every field has a size, using uint32_t for each register ensures that each register is 32 bits wide, which matches the hardware specification for offsets and sizes. 
 *
 * DATA is the first field, so it is at offset 0x00, STATE is at offset 0x04, CTRL at 0x08, INTSTATUS at 0x0C and BAUDDIV at 0x10. This layout allows us to access the registers using pointers.
 * */

typedef struct {
    volatile uint32_t DATA;      // Offset 0x00: Data register for transmit/receive
    volatile uint32_t STATE;     // Offset 0x04: Status register (TX_FULL, RX_FULL, TX_OVERRUN, RX_OVERRUN)
    volatile uint32_t CTRL;      // Offset 0x08: Control register (TX_EN, RX_EN, TX_INT_EN, RX_INT_EN, TX_OVR_INT_EN, RX_OVR_INT_EN, HS_TM)
    volatile uint32_t INTSTATUS; // Offset 0x0C: Interrupt status register (read-only, reflects pending interrupts)
    volatile uint32_t BAUDDIV;   // Offset 0x10: Baud rate divider register
} UART_TypeDef;

/*
 * Define pointers to each UART peripheral: I need this pointers to access the UART registers separately without repeating the same code. Basically, each point is a pointer to a UART_TypeDef struct, but with a differente base address. This way I'm saying to compiled to point to different memory locations of the hardware. Using UART0->DATA, for example, will access the DATA register of UART0 and access it's content this is the same for other UART peripherals.
 *
 * Something like &(UART0->DATA) will give the address of the register. Useful information because UART0->DATA gives its content. 
 *
 * */
#define UART0 ((UART_TypeDef *) UART0_BASE) 
#define UART1 ((UART_TypeDef *) UART1_BASE) 
#define UART2 ((UART_TypeDef *) UART2_BASE) 
#define UART3 ((UART_TypeDef *) UART3_BASE) 
#define UART4 ((UART_TypeDef *) UART4_BASE) 
 
/*
 * Bits definitions: This is very straight forward, I defined some macros for the bits in the control and status registers to make the code more readable.
 * */
/* UART_STATE bits */
#define UART_STATE_TX_FULL    (1U << 0)
#define UART_STATE_RX_FULL    (1U << 1)
#define UART_STATE_TX_OVERRUN (1U << 2)
#define UART_STATE_RX_OVERRUN (1U << 3)


// uart_init(): This function initializes the UART peripheral by configuring the control register to enable the transmitter and receiver. 
void uart_init(void);

#endif
