// multimod_joystick.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for joystick functions

/************************************Includes***************************************/

#include "../multimod_joystick.h"

#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <driverlib/adc.h>
#include <driverlib/interrupt.h>

#include <inc/tm4c123gh6pm.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_i2c.h>
#include <inc/hw_gpio.h>

/********************************Public Functions***********************************/

// JOYSTICK_Init
// Initializes ports & adc module for joystick
// Return: void
void JOYSTICK_Init(void) {
    // Disable adc
    SysCtlPeripheralDisable(SYSCTL_PERIPH_ADC0);
    //Joy x PE3
    //Joy y PE2
    // Enable gpio port
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    // Enable adc module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    // Set pins as ADC
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    // Configure ADC sequences
    //ADCSequenceConfigure(ADC0_BASE, );
    ADCSequenceConfigure(ADC0_BASE, 2 , ADC_TRIGGER_PROCESSOR, 2);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);
    // Enable ADC sequence
    ADCSequenceEnable(ADC0_BASE, 2);
}

// JOYSTICK_IntEnable
// Enables interrupts
// Return: void
void JOYSTICK_IntEnable() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, JOYSTICK_INT_PIN);
    GPIOIntTypeSet(JOYSTICK_INT_GPIO_BASE, JOYSTICK_INT_PIN, GPIO_FALLING_EDGE);
    GPIOIntEnable(JOYSTICK_INT_GPIO_BASE, JOYSTICK_INT_PIN);
}

// JOYSTICK_GetPress
// Gets button reading
// Return: bool
uint8_t JOYSTICK_GetPress() {
    if (GPIOPinRead(JOYSTICK_INT_GPIO_BASE, JOYSTICK_INT_PIN)) {
        return 0;
    }

    return 1;
}

// JOYSTICK_GetX
// Gets X adc reading from joystick
// Return: uint16_t
uint16_t JOYSTICK_GetX() {
    uint32_t result = JOYSTICK_GetXY();

    return (result >> 16 & 0xFFFF);
}

// JOYSTICK_GetY
// Gets Y adc reading from joystick
// Return: uint16_t
uint16_t JOYSTICK_GetY() {
    uint32_t result = JOYSTICK_GetXY();

    return (result >> 0 & 0xFFFF);
}


// JOYSTICK_GetXY
// Gets X and Y adc readings
// Return: uint32_t, 16-bit packed, upper 16-bit is X and lower 16-bit is Y.
uint32_t JOYSTICK_GetXY() {
    uint32_t results[2];

    // Start conversion
    ADCProcessorTrigger(ADC0_BASE, 2);

    // Wait until conversion is complete
    while(!ADCIntStatus(ADC0_BASE, 2, 0));

    // Clear ADC interrupt flag
    ADCIntClear(ADC0_BASE, 2);

    // Read from x and y coordinates
    ADCSequenceDataGet(ADC0_BASE, 2, results);

    return (results[0] << 16 | results[1]);
}


