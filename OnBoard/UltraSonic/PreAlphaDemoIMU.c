

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


int main(void) {

    multimod_init();

    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    G8RTOS_Init();
    multimod_init();

    G8RTOS_InitSemaphore(&sem_uart, 1);
    G8RTOS_InitSemaphore(&sem_I2CA, 1);
    G8RTOS_AddThread(Idle_Thread, 255, "idle\0");
    G8RTOS_AddThread(Gyro_Thread, 5, "camera\0");
    G8RTOS_AddThread(Accel_Thread, 5, "buttons\0");

        G8RTOS_Launch();
        while (1);
}


