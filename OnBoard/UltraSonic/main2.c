
/* -----------------------          Include Files       --------------------- */
#include <stdint.h>                         // Library of Standard Integer Types
#include <stdbool.h>                        // Library of Standard Boolean Types
#include <stdlib.h>                         // Library of Standard Datatype Conversions
#include "inc/tm4c123gh6pm.h"               // Definitions for interrupt and register assignments on Tiva C
#include "inc/hw_memmap.h"                  // Macros defining the memory map of the Tiva C Series device
#include "inc/hw_types.h"                   // Defines common types and macros
#include "inc/hw_timer.h"                   // Defines and macros used when accessing the timer
#include "inc/hw_gpio.h"                    // Defines Macros for GPIO hardware
#include "driverlib/debug.h"                // Macros for assisting debug of the driver library
#include "driverlib/sysctl.h"               // Defines and macros for System Control API of DriverLib
#include "driverlib/interrupt.h"            // Defines and macros for NVIC Controller API of DriverLib
#include "driverlib/timer.h"                // Defines and macros for Timer API of driverLib
#include "driverlib/gpio.h"                 // Defines and macros for GPIO API of DriverLib
#include "driverlib/pin_map.h"              // Mapping of peripherals to pins for all parts
#include "driverlib/uart.h"                 // Defines and Macros for the UART
#include "driverlib/rom.h"                  // Defines and macros for ROM API of driverLib
#include "MultimodDrivers/multimod_uart.h"
#define UART0_BAUDRATE  115200              // Macro for UART0 Baud rate

/* -----------------------      Global Variables        --------------------- */
volatile bool boolTrigCondition = 1;        // Variable to control the Trigger Pin Switching
volatile bool checkEcho = 0;
volatile uint32_t ui32EchoDuration = 0;     // Variable to store duration for which Echo Pin is high
volatile uint32_t ui32ObstacleDist = 0;     // Variable to store distance of the Obstacle

/* -----------------------      Function Prototypes     --------------------- */
void Timer0IntHandler(void);                // The prototype of the ISR for Timer0 Interrupt
void PortAIntHandler(void);                 // Prototype for ISR of GPIO PortA
void TrigDelay(void);

/* -----------------------          Main Program        --------------------- */

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
int main(void){
    // Set the System clock to 80MHz and Enable the clock for peripherals PortA, Timer0, Timer2 and UART0
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

    // Master interrupt enable API for all interrupts
    IntMasterDisable();

    UART_Init();

    // Set the PA3 port as Output. Trigger Pin
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
    // Set the PA2 port as Input with a weak Pull-down. Echo Pin
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPD);
    // Configure and enable the Interrupt on both edges for PA2. Echo Pin
    IntEnable(INT_GPIOA);
    IntRegister(INT_GPIOA, PortAIntHandler);
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_RISING_EDGE);
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_2);

    // Configure Timer0 to run in one-shot down-count mode
    TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT);
    // Enable the Interrupt specific vector associated with Timer0A
    IntEnable(INT_TIMER0A);
    IntRegister(INT_TIMER0A, Timer0AIntHandler);
    // Enables a specific event within the timer to generate an interrupt
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Configure Timer2 to run in one-shot up-count mode
    TimerConfigure(TIMER2_BASE, TIMER_CFG_ONE_SHOT_UP);

    uint8_t iter;
    IntMasterEnable();

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
                // Print
                UARTprintf("distance: %d\n", ui32ObstacleDist);
                // Enable condition for Trigger Pulse
                boolTrigCondition = 1;
                checkEcho = 0;
                GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_2);
            }
        }
    }
}

/* -----------------------      Function Definition     --------------------- */
