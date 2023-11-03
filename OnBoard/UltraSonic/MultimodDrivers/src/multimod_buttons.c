// multimod_buttons.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for button functions

/************************************Includes***************************************/

#include "../multimod_buttons.h"

#include "../multimod_i2c.h"

#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>

#include <inc/tm4c123gh6pm.h>
#include <inc/hw_i2c.h>

/********************************Public Functions***********************************/

// Buttons_Init
// Initializes buttons on the multimod by configuring the I2C module and
// relevant interrupt pin.
// Return: void
void MultimodButtons_Init()
{
    I2C_Init(I2C_A_BASE);
    // Initialize this function & the relevant interrupt pin
    //interrupt on PE4
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);

    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_4);


    /*
    // >7 priority
    uint8_t toWrite[2];
    toWrite[0] = 6;
    toWrite[1] = 0xff;
    I2C_WriteMultiple(I2C_A_BASE, 0x23, toWrite, 2);//all inputs
    */
    //slave address 0x23
    return;
}

// MultimodButtons_Get
// Gets the input to GPIO bank 1, [0..7].
// Return: uint8_t 
uint8_t MultimodButtons_Get() {
    // complete this function
    I2C_WriteSingle(I2C_A_BASE, 0x23, 0x0);
    while(I2CMasterBusy(I2C_A_BASE));
    uint8_t output = I2C_ReadSingle(I2C_A_BASE, 0x23);
    return ~output;
}

