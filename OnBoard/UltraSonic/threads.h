#ifndef THREADS_H_
#define THREADS_H_


#include "./G8RTOS/G8RTOS.h"

semaphore_t sem_I2CA;
semaphore_t sem_uart;
semaphore_t sem_sensor;


void Idle_Thread(void);
void Gyro_Thread(void);
void Accel_Thread(void);
void Ultrasonic_Thread(void);
void PortAIntHandler(void);
void Timer0AIntHandler(void);
void UART4Handler(void);

#endif /* THREADS_H_ */
