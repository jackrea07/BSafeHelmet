#include "./threads.h"

#include "./MultimodDrivers/multimod.h"
#include "driverlib/timer.h"
#include "inc/hw_timer.h"                   // Defines and macros used when accessing the timer


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

volatile bool boolTrigCondition = 1;        // Variable to control the Trigger Pin Switching
volatile bool checkEcho = 0;
volatile uint32_t ui32EchoDuration = 0;     // Variable to store duration for which Echo Pin is high
volatile uint32_t ui32ObstacleDist = 0;     // Variable to store distance of the Obstacle
volatile uint32_t noCount = 0;
volatile uint32_t obstCount = 0;

void Idle_Thread(void) {
    time_t t;
    srand((unsigned) time(&t));
    while(1);
}

void Accel_Thread(void){
    while(1){
        G8RTOS_WaitSemaphore(&sem_I2CA);
        int16_t x_accel_value = BMI160_AccelXGetResult();
        G8RTOS_SignalSemaphore(&sem_I2CA);
        G8RTOS_WaitSemaphore(&sem_uart);
        UARTprintf("X Accel Value: %d\n", x_accel_value);
        G8RTOS_SignalSemaphore(&sem_uart);
        sleep(1000);
    }
}

void Gyro_Thread(void){
    while(1){
        G8RTOS_WaitSemaphore(&sem_I2CA);
        int16_t x_gyro_value = BMI160_GyroXGetResult();
        x_gyro_value = abs(x_gyro_value);
        G8RTOS_SignalSemaphore(&sem_I2CA);
        if (x_gyro_value > 5000){
            G8RTOS_WaitSemaphore(&sem_uart);
            UARTprintf("Crash Detected! Gyro Value: %d\n", x_gyro_value);
            G8RTOS_SignalSemaphore(&sem_uart);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0);
            SysCtlDelay(10000000);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);
        }
        sleep(20);
    }
}

void Ultrasonic_Thread(void){
    while (1){
        if (boolTrigCondition){
            // Load the Timer with value for generating a  delay of 10 uS.
            TimerLoadSet(TIMER0_BASE, TIMER_A, (SysCtlClockGet() / 100000) -10);
            // Make the Trigger Pin (PA3) High
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
            // Enable the Timer0 to cause an interrupt when timeout occurs
            TimerEnable(TIMER0_BASE, TIMER_A);
            // Disable the condition for Trigger Pin Switching
            boolTrigCondition = 0;
            //for (iter = 0; iter<sizeof(itworked); iter++ ) UARTCharPut(UART0_BASE, itworked[iter]);
        }
        while(checkEcho){
            if (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2) == 0){
                ui32EchoDuration = TimerValueGet(TIMER2_BASE, TIMER_A);
                // Disable Timer2 to stop measuring duration for which Echo Pin is High
                TimerDisable(TIMER2_BASE, TIMER_A);
                // Convert the Timer Duration to Distance Value according to Ultrasonic's formula
                ui32ObstacleDist = ui32EchoDuration / 4640;

                if(ui32ObstacleDist > 30 && ui32ObstacleDist < 450){
                    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4);
                }
                else {
                    obstCount+=1;
                    //GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0);
                }
                if(obstCount > 10){
                    obstCount = 0;
                    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0);
                }
                // Print
                G8RTOS_WaitSemaphore(&sem_uart);
                UARTprintf("distance: %d\n", ui32ObstacleDist);
                G8RTOS_SignalSemaphore(&sem_uart);
                // Enable condition for Trigger Pulse
                boolTrigCondition = 1;
                checkEcho = 0;
                GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_2);
                sleep(100);
            }
        }
    }
}

/*void Ultrasonic_Thread(void){
    uint32_t samples[10] = {0,0,0,0,0,0,0,0,0,0};
    uint8_t index = 0;
    uint32_t average = 0;
    while (1){
        if (boolTrigCondition){
            // Load the Timer with value for generating a  delay of 10 uS.
            TimerLoadSet(TIMER0_BASE, TIMER_A, (SysCtlClockGet() / 100000) -10);
            // Make the Trigger Pin (PA3) High
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
            // Enable the Timer0 to cause an interrupt when timeout occurs
            TimerEnable(TIMER0_BASE, TIMER_A);
            // Disable the condition for Trigger Pin Switching
            boolTrigCondition = 0;
            //for (iter = 0; iter<sizeof(itworked); iter++ ) UARTCharPut(UART0_BASE, itworked[iter]);
        }
        while(checkEcho){
            if (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2) == 0){
                 while(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2) != 0);
                ui32EchoDuration = TimerValueGet(TIMER2_BASE, TIMER_A);
                // Disable Timer2 to stop measuring duration for which Echo Pin is High
                TimerDisable(TIMER2_BASE, TIMER_A);
                // Convert the Timer Duration to Distance Value according to Ultrasonic's formula
                ui32ObstacleDist = ui32EchoDuration / 4640;
                // Print
                if(index >= 10)
                {
                    index = 0;
                }
                if(ui32ObstacleDist < 1200)
                {
                    samples[index] = ui32ObstacleDist;
                    index++;
                }
                //add to

                for(uint8_t x = 0; x < 10; x++)
                {
                    average+= samples[x];
                }
                average /= 10;



                G8RTOS_WaitSemaphore(&sem_uart);
                UARTprintf("distance: %d\n", average);
                G8RTOS_SignalSemaphore(&sem_uart);
                // Enable condition for Trigger Pulse
                boolTrigCondition = 1;
                checkEcho = 0;
                GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_2);
                sleep(100);
            }
        }
    }
}*/

void Timer0AIntHandler(void){
    // The ISR for Timer0 Interrupt Handling
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Disable the timer
    TimerDisable(TIMER0_BASE, TIMER_A);
    // Make the Trigger Pin (PA3) Low
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
}

void PortAIntHandler(void){
    // The ISR for GPIO PortA Interrupt Handling
    // Clear the GPIO Hardware Interrupt
    checkEcho = 1;
    GPIOIntClear(GPIO_PORTA_BASE , GPIO_INT_PIN_2);

    // Initialize Timer2 with value 0
    HWREG(TIMER2_BASE + TIMER_O_TAV) = 0;
    // Enable Timer2 to start measuring duration for which Echo Pin is High
    TimerEnable(TIMER2_BASE, TIMER_A);
    GPIOIntDisable(GPIO_PORTA_BASE, GPIO_INT_PIN_2);
}
