#include "./threads.h"

#include "./MultimodDrivers/multimod.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
        if (x_gyro_value > 10000){
            G8RTOS_WaitSemaphore(&sem_uart);
            UARTprintf("Crash Detected! Gyro Value: %d\n", x_gyro_value);
            G8RTOS_SignalSemaphore(&sem_uart);
        }
        sleep(10);
    }
}

/*void Ultrasonic_Thread(void){

}*/
