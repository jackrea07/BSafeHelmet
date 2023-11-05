

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

/************************************Includes***************************************/

/*************************************Defines***************************************/
/*************************************Defines***************************************/

/********************************Public Variables***********************************/

uint8_t read_imu_flag = 0;
uint8_t read_opt_flag = 0;
uint8_t toggle_led_flag = 0;
uint8_t print_uart_flag = 0;

/********************************Public Variables***********************************/

/********************************Public Functions***********************************/

void TIMER0A_Handler(void);
void TIMER0B_Handler(void);
void TIMER1A_Handler(void);
void TIMER1B_Handler(void);

void Timer_Init() {
    // Initialize timers
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1));

    // Disable timers
    TimerDisable(TIMER0_BASE, TIMER_BOTH);
    TimerDisable(TIMER1_BASE, TIMER_BOTH);
    // Configure timers as half-width, periodic 16-bit or (32-bit if using 64-bit timers) timers for a total of 4 timers

    TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC));
    TimerConfigure(TIMER1_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC));

    // Set prescalers
    TimerPrescaleSet(TIMER0_BASE, TIMER_A, 255);
    TimerPrescaleSet(TIMER0_BASE, TIMER_B, 255);
    TimerPrescaleSet(TIMER1_BASE, TIMER_A, 255);
    TimerPrescaleSet(TIMER1_BASE, TIMER_B, 255);

    // Load initial timer values
        // Sysclock / prescaler * desired seconds = timer period
    TimerLoadSet(TIMER0_BASE, TIMER_A, (SysCtlClockGet()/255 /2) - 1);//uart out every 500 ms
    TimerLoadSet(TIMER0_BASE, TIMER_B, (SysCtlClockGet()/255 /10) - 1);//sample x-axis acceleration from BMI160 every 100 ms
    TimerLoadSet(TIMER1_BASE, TIMER_A, ((SysCtlClockGet()/255 * 15 )/100) - 1);//sample light intensity from OPT3001 every 150 ms
    TimerLoadSet(TIMER1_BASE, TIMER_B, (SysCtlClockGet()/255 /10) - 1);//toggle the red LED every 100 ms

    // Enable timer interrupts
    IntEnable(INT_TIMER0A);
    IntEnable(INT_TIMER0B);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT); //def need this for each of the timers

    IntEnable(INT_TIMER1A);
    IntEnable(INT_TIMER1B);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT);
    // Enable timers
    TimerEnable(TIMER0_BASE, TIMER_BOTH);
    TimerEnable(TIMER1_BASE, TIMER_BOTH);
}

void Int_Init(void) {
    IntMasterDisable();

    // Enable timer interrupt, set interrupt priorities
    IntEnable(INT_TIMER0A);
    IntPrioritySet(INT_TIMER0A, 1);//0.5
    IntEnable(INT_TIMER0B);
    IntPrioritySet(INT_TIMER0B, 8);//0.1
    IntEnable(INT_TIMER1A);
    IntPrioritySet(INT_TIMER1A, 5);//0.15
    IntEnable(INT_TIMER1B);
    IntPrioritySet(INT_TIMER1B, 8);//0.1

    // Point to relevant timer handler function
    IntRegister(INT_TIMER0A, TIMER0A_Handler);
    IntRegister(INT_TIMER0B, TIMER0B_Handler);
    IntRegister(INT_TIMER1A, TIMER1A_Handler);
    IntRegister(INT_TIMER1B, TIMER1B_Handler);

    IntMasterEnable();
}

void LED_Init(void) {
    // Enable clock to port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Set pins [2..0] to output, set as digital
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1, 0x00);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1);
}


/********************************Public Functions***********************************/

/************************************MAIN*******************************************/
// Use timers to enforce specific schedules for each event.
int main(void) {
    Int_Init();
    Timer_Init();
    UART_Init();

    I2C_Init(I2C_A_BASE);
    LED_Init();
    BMI160_InitAlpha();
    OPT3001_Init();

    int16_t x_accel_value = 0;
    uint16_t opt_value = 0;
    uint8_t red_flag = 0;
    while(1) {
        // Write code to read the x-axis accelerometer value,
        // opt3001 sensor, toggle the red led, and print
        // values out to the console when needed.
        if(read_imu_flag){ //0.1
            x_accel_value = BMI160_AccelXGetResult();
            UARTprintf("X Accel Value: %d\n", x_accel_value);
            read_imu_flag = 0;
        }
        if(read_opt_flag){ //0.15
            int16_t result = BMI160_GyroXGetResult();
            UARTprintf("Gyro Value: %d\n", result);
            read_opt_flag = 0;
        }
        if(toggle_led_flag){ //0.1
            if(red_flag == 0){
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xFF);
                red_flag = 1;
            }
            if(red_flag == 1){
                //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xFF);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
                red_flag = 0;
            }
            //UARTprintf("Toggle LED every 100 ms\n");
            toggle_led_flag = 0;
        }
        //if(print_uart_flag){ //0.5
            //UARTprintf("Print UART every 500 ms. Hello world!\n");
          //  print_uart_flag = 0;
        //}
    }
}


/************************************MAIN*******************************************/

/********************************Public Functions***********************************/

/*******************************Interrupt Handlers**********************************/

// Timer handlers are provided to you.
void TIMER0A_Handler(void) {
    print_uart_flag = 1;
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    return;
}

void TIMER0B_Handler(void) {
    read_imu_flag = 1;
    TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
    return;
}

void TIMER1A_Handler(void) {
    read_opt_flag = 1;
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    return;
}

void TIMER1B_Handler(void) {
    toggle_led_flag = 1;
    TimerIntClear(TIMER1_BASE, TIMER_TIMB_TIMEOUT);
    return;
}
/*******************************Interrupt Handlers**********************************/
