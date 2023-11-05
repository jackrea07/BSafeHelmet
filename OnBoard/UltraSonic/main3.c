/*
 * main3.c
 *
 *  Created on: Nov 3, 2023
 *      Author: radbrad
 */
/*
#include <stdio.h>
#include <stdlib.h>
#include "./MultimodDrivers/multimod.h"
#include "TM4C123GH6PM.h"
#include <inc/hw_sysctl.h>"
#include <driverlib/timer.h>
#include <driverlib/pin_map.h>
*/
#include <stdint.h>
#include <stdbool.h>

#include <inc/tm4c123gh6pm.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include "./MultimodDrivers/multimod.h"
#include <driverlib/uartstdio.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>

void HC05_Init();

void Delay(unsigned delay){
    unsigned i;
    for(i = 0; i < delay; i++);
}
int main(void){
    HC05_Init();
    //UART_Init();
    Delay(1000000);
    UARTprintf("test bluetooth\n");

}

void HC05_Init(){
    // enables UART2 on GPIOD pins 6 & 7
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    // Enable UART2 module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);

    // Configure UART0 pins on port A
    GPIOPinConfigure(GPIO_PD6_U2RX);
    GPIOPinConfigure(GPIO_PD7_U2TX);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    // Set UART clock source
    UARTClockSourceSet(UART2_BASE, UART_CLOCK_SYSTEM);

    // Configure UART baud rate
    UARTStdioConfig(2, 115200, SysCtlClockGet());
}
/*void LED_Init(){
    GPIOPinConfigure
}*/
