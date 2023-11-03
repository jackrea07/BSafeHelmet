// multimod_uart.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for UART functions

/************************************Includes***************************************/

#include "../multimod_uart.h"

#include <stdint.h>
#include <stdbool.h>

#include <inc/tm4c123gh6pm.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>

#include <driverlib/uartstdio.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>

/************************************Includes***************************************/

/********************************Public Functions***********************************/

// UART_Init
// Initializes UART serial communication with PC
// Return: void
void UART_Init() {
    // This should have been done in lab 0, so it's just copy & paste.
    // Enable port A

    SYSCTL_RCGCUART_R |= 0x01;
    SYSCTL_RCGCGPIO_R |= 0x01;

    // Configure UART0 pins on port A
    GPIO_PORTA_DEN_R |= 0x03;
    GPIO_PORTA_AFSEL_R |= 0x03;
    GPIO_PORTA_PCTL_R |= 0x11;


    UARTStdioConfig(0,9600,SysCtlClockGet());

    SysCtlDelay(10000);
}

/********************************Public Functions***********************************/

