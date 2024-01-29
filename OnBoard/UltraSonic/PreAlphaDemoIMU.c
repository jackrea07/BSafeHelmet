

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
#include <driverlib/interrupt.h>
#include <driverlib/timer.h>

#include "MultimodDrivers/multimod.h"
#include "threads.h"

void Timers_Init(void);
void GPIO_Init(void);

void Timers_Init(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    // Configure Timer0 to run in one-shot down-count mode
    TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT);
    // Enable the Interrupt specific vector associated with Timer0A
    IntEnable(INT_TIMER0A);
    //IntRegister(INT_TIMER0A, Timer0AIntHandler);
    // Enables a specific event within the timer to generate an interrupt
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Configure Timer2 to run in one-shot up-count mode
    TimerConfigure(TIMER2_BASE, TIMER_CFG_ONE_SHOT_UP);
}

void GPIO_Init(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
    //GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
    // Set the PA2 port as Input with a weak Pull-down. Echo Pin
    //GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2);
    //GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPD);
    // Configure and enable the Interrupt on both edges for PA2. Echo Pin
    //IntEnable(INT_GPIOA);
    //IntRegister(INT_GPIOA, PortAIntHandler);
    //GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_RISING_EDGE);
    //GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPD);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);
}

int main(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    G8RTOS_Init();
    IntMasterDisable();
    multimod_init();


    GPIO_Init();
    //Timers_Init();
    IntMasterEnable();

    G8RTOS_InitSemaphore(&sem_uart, 1);
    G8RTOS_InitSemaphore(&sem_I2CA, 1);
    G8RTOS_InitSemaphore(&sem_sensor, 0);
    G8RTOS_AddThread(Idle_Thread, 255, "idle\0");
    G8RTOS_AddThread(Gyro_Thread, 5, "camera\0");
    G8RTOS_AddThread(Accel_Thread, 6, "buttons\0");
    G8RTOS_AddThread(Ultrasonic_Thread, 4, "ultrasonic\0");

    G8RTOS_Launch();
    while (1);
}


