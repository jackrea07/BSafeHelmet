// multimod_OPT3001.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for OPT3001 functions

/************************************Includes***************************************/

#include "../multimod_LaunchpadButtons.h"

#include <stdint.h>
#include <stdbool.h>

#include <inc/tm4c123gh6pm.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_i2c.h>
#include <inc/hw_gpio.h>

#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>

/************************************Includes***************************************/

/********************************Public Functions***********************************/

// LaunchpadButtons_Init
// Initializes the GPIO port & pins necessary for the button switches on the
// launchpad.
// Return: void
void LaunchpadButtons_Init() {
    // Enable port F for switches
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Use SW1 & SW2, configure as inputs.
    GPIO_PORTF_LOCK_R = 0x4C4F434B;//unlock PF
    GPIO_PORTF_CR_R = 0x01;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);//Switch 1
    GPIO_PORTF_PUR_R |= 0x11;//pull up, and I'll drop ya Im the illest
    // Pay attention to the pin connected to SW2!!! PF0 is a locked pin.
    //GPIOLOCK register
}

// LaunchpadButtons_ReadSW1
// Checks to see if button 1 is pressed. If it is, return 1; else return 0.
// Return: uint8_t
uint8_t LaunchpadButtons_ReadSW1() {
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)
    {
    return 1;
    }
    else{
        return 0;
    }
}

// LaunchpadButtons_ReadSW2
// Checks to see if button 1 is pressed. If it is, return 1; else return 0.
// Return: uint8_t
uint8_t LaunchpadButtons_ReadSW2() {

    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0)
        {
        return 1;
        }
        else{
            return 0;
        }

}



/********************************Public Functions***********************************/
