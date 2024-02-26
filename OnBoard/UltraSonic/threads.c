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
volatile float accel_max_x = 0;
volatile float accel_max_y = 0;
volatile float accel_max_z = 0;
volatile float accel_neg_max_x = 0;
volatile float accel_neg_max_y = 0;
volatile float accel_neg_max_z = 0;
volatile float gyro_max_x = 0;
volatile float gyro_max_y = 0;
volatile float gyro_max_z = 0;
volatile uint16_t count0 = 0;
volatile uint16_t count1 = 0;
volatile float average0 = 0;
volatile float average1 = 0;
char data[3];
uint8_t index = 0;
uint32_t ADC0Values[1];
uint32_t ADC1Values[1];
float distance0 = 0;
float distance1 = 0;

void Idle_Thread(void) {
    time_t t;
    srand((unsigned) time(&t));
    while(1);
}

void Accel_Thread(void){
    while(1){
        G8RTOS_WaitSemaphore(&sem_I2CA);
        float x_accel_value = (float)BMI160_AccelXGetResult()/16630;
        if (x_accel_value > accel_max_x){
            accel_max_x = x_accel_value;
        }
        if (x_accel_value < accel_neg_max_x){
            accel_neg_max_x = x_accel_value;
        }
        float y_accel_value = (float)BMI160_AccelYGetResult()/16630;
        if (y_accel_value > accel_max_y){
            accel_max_y = y_accel_value;
        }
        if (y_accel_value < accel_neg_max_y){
            accel_neg_max_y = y_accel_value;
        }
        float z_accel_value = (float)BMI160_AccelZGetResult()/16630;
        if (z_accel_value > accel_max_z){
            accel_max_z = z_accel_value;
        }
        if (z_accel_value < accel_neg_max_z){
            accel_neg_max_z = z_accel_value;
        }
        if(x_accel_value > 1.7 || y_accel_value > 1.7 || z_accel_value > 1.7 || x_accel_value < -1.7 || y_accel_value < -1.7 || z_accel_value < -1.7){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);
            SysCtlDelay(10000000);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0);
        }
        G8RTOS_SignalSemaphore(&sem_I2CA);
        G8RTOS_WaitSemaphore(&sem_uart);
        //UARTprintf("X Accel Value: %d\n", x_accel_value);
        G8RTOS_SignalSemaphore(&sem_uart);
        sleep(20);
    }
}

void Gyro_Thread(void){
    while(1){
        G8RTOS_WaitSemaphore(&sem_I2CA);
        int16_t x_gyro_value = BMI160_GyroXGetResult();
        x_gyro_value = abs(x_gyro_value);
        if (x_gyro_value > gyro_max_x){
            gyro_max_x = x_gyro_value;
        }
        int16_t y_gyro_value = BMI160_GyroYGetResult();
        y_gyro_value = abs(y_gyro_value);
        if (y_gyro_value > gyro_max_y){
            gyro_max_y = y_gyro_value;
        }
        int16_t z_gyro_value = BMI160_GyroZGetResult();
        z_gyro_value = abs(z_gyro_value);
        if (z_gyro_value > gyro_max_z){
            gyro_max_z = z_gyro_value;
        }
        G8RTOS_SignalSemaphore(&sem_I2CA);
        if (x_gyro_value > 5000){
            G8RTOS_WaitSemaphore(&sem_uart);
            //UARTprintf("Crash Detected! Gyro Value: %d\n", x_gyro_value);
            G8RTOS_SignalSemaphore(&sem_uart);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);
            SysCtlDelay(10000000);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0);
        }
        sleep(20);
    }
}

void Ultrasonic_Thread(void){
    while(1){
        ADCProcessorTrigger(ADC0_BASE, 3);
        while(!ADCIntStatus(ADC0_BASE, 3, false));
        ADCIntClear(ADC0_BASE, 3);
        ADCSequenceDataGet(ADC0_BASE, 3, ADC0Values);
        distance0 = ((((float)ADC0Values[0])/1029)*1000)/9.8;
        if (distance0 < 80)
        {
            //UARTprintf("first: %d\n", (int32_t)distance0);
            count0++;
        }
        else
        {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0);
            count0 = 0;
        }
        if(count0 > 25){
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4);
            count0 = 0;
        }
        //////////////////////////////////////////////////
        ADCProcessorTrigger(ADC1_BASE, 3);
        while(!ADCIntStatus(ADC1_BASE, 3, false));
        ADCIntClear(ADC1_BASE, 3);
        ADCSequenceDataGet(ADC1_BASE, 3, ADC1Values);
        distance1 = ((((float)ADC1Values[0])/1029)*1000)/9.8;
        //UARTprintf("second: %d\n", (int32_t)distance1);
        //if(distance1 < 80){
        if(distance1 < 80){
            count1++;
        }
        else{
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);
            count1 = 0;
        }
        if(count1 > 25){
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
            count1 = 0;
        }
    sleep(20);
    }
}




