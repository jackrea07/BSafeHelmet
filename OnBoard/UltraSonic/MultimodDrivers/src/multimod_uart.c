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
#include <driverlib/adc.h>

/************************************Includes***************************************/

/********************************Public Functions***********************************/

// UART_Init
// Initializes UART serial communication with PC
// Return: void
void UART_Init() {
    // This should have been done in lab 0, so it's just copy & paste.
// Enable port A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    // Enable UART0 module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Configure UART0 pins on port A

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTFIFODisable(UART0_BASE);

    // Set UART clock source
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);
    // Configure UART baud rate

    UARTStdioConfig(0, 115200, SysCtlClockGet());
}

void ADC_INIT(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlDelay(3);

    ADCReferenceSet(ADC0_BASE, ADC_REF_INT);

    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH1 | ADC_CTL_IE |
                                     ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 3);

    ADCIntClear(ADC0_BASE, 3);
}

void ADC1_INIT(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    SysCtlDelay(3);

    ADCReferenceSet(ADC1_BASE, ADC_REF_INT);

    ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE |
                                         ADC_CTL_END);

    ADCSequenceEnable(ADC1_BASE, 3);

    ADCIntClear(ADC1_BASE, 3);
}

/********************************Public Functions***********************************/

