// multimod_PCA9956b.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for PCA9956b functions

/************************************Includes***************************************/

#include "../multimod_PCA9956b.h"


#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>

#include <stdint.h>
#include "../multimod_i2c.h"

/********************************Public Functions***********************************/

// PCA9956b_Init
// Initializes the PCA9956b, initializes the relevant output enable pins
// Return: void
void PCA9956b_Init() {
    I2C_Init(I2C_A_BASE);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, OE_PIN);
    GPIOPinWrite(GPIO_PORTD_BASE, OE_PIN, 1);

    PCA9956b_SetAllOff();

    return;
}

// PCA9956b_SetAllMax
// Writes to the IREFALL, PWMALL registers, sets LEDs to maximum.
// Return: void
void PCA9956b_SetAllMax() {
    uint8_t bytes[] = {(~(AI_BIT) & IREFALL), 0xFF};
    I2C_WriteMultiple(I2C_A_BASE, PCA9956B_ADDR, bytes, 2);

    bytes[0] = (~(AI_BIT) & PWMALL);
    I2C_WriteMultiple(I2C_A_BASE, PCA9956B_ADDR, bytes, 2);
}

// PCA9956b_SetAllOff
// Writes to the IREFALL, PWMALL registers, sets LEDs to off.
// Return: void
void PCA9956b_SetAllOff() {
    uint8_t bytes[] = {(~(AI_BIT) & IREFALL), 0x00};
    I2C_WriteMultiple(I2C_A_BASE, PCA9956B_ADDR, bytes, 2);

    bytes[0] = (~(AI_BIT) & PWMALL);
    I2C_WriteMultiple(I2C_A_BASE, PCA9956B_ADDR, bytes, 2);
}

// PCA9956b_EnableOutput
// Sets output enable pin to true.
// Return: void
void PCA9956b_EnableOutput() {
    GPIOPinWrite(GPIO_PORTD_BASE, OE_PIN, 0);
}

// PCA9956b_EnableOutput
// Sets output enable pin to false.
// Return: void
void PCA9956b_DisableOutput() {
    GPIOPinWrite(GPIO_PORTD_BASE, OE_PIN, 1);
}

uint8_t PCA9956b_GetChipID() {
    I2C_WriteSingle(I2C_A_BASE, PCA9956B_ADDR, 0x01);
    return I2C_ReadSingle(I2C_A_BASE, PCA9956B_ADDR);
}
