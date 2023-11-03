#include <stdio.h>
#include <stdlib.h>
#include "./MultimodDrivers/multimod.h"
#include "TM4C123GH6PM.h"
#include <inc/hw_sysctl.h>"
#include <driverlib/timer.h>
#include <driverlib/pin_map.h>
/**
 * main.c
 */
int main(void)
{
	UART_Init();
	Timer0ACaptureInit();
	while(1)
	{
	    UARTprintf("test");
	}
}

void Timer0ACaptureInit()
{
    //enable clock to timer block 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    //enable clock to port b/a
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    //port b input pin
    GPIOPinTypeGPIOInput(SYSCTL_PERIPH_GPIOB, GPIO_INT_PIN_6);
    //configure as digital alternate function for T0CCP0
    GPIOPinConfigure(GPIO_PB6_T0CCP0);

    //pb2 as digital output pin
    GPIOPinTypeGPIOOutput(SYSCTL_PERIPH_GPIOB, GPIO_INT_PIN_2);
    //Disable timer a
    TimerDisable(TIMER0_BASE, TIMER_A);
//timer a 16 bit count up, capture count up
    TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT_UP));
//rising edge capture
    TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);

    TimerEnable(TIMER0_BASE, TIMER_A);
}

uint32_t measure()
{
    SysCtlDelay(10);
    return 0;
}
