#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "../ultrasonic_driver.h"

volatile uint32_t pulseWidth;  // Variable to store the pulse width

void ultrasonic_init(void) {
    // Initialize GPIO pins for trigger and echo
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);  // Trigger pin
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_2);   // Echo pin

    // Initialize Timer1A for capturing echo pulse width
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP);
    TimerControlEvent(TIMER1_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);
    TimerEnable(TIMER1_BASE, TIMER_A);
}

void ultrasonicTimer_init(void) {
    // Initialize Timer0A for triggering ultrasonic sensor
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 1000);  // 1 ms interval
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);

    IntEnable(INT_TIMER0A);  // Enable Timer0A interrupt
    IntMasterEnable();       // Enable the overall interrupt flag
}

void ultrasonic_trigger(void) {
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);  // Set trigger pin high
    SysCtlDelay(16000);  // Small delay for trigger pulse (adjust as needed)
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);  // Clear trigger pin to generate pulse
}

uint32_t ultrasonic_get(void) {
    // Wait for echo pin to go high
    while (!GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2));

    // Start Timer1A
    TimerLoadSet(TIMER1_BASE, TIMER_A, 0);
    TimerEnable(TIMER1_BASE, TIMER_A);

    // Wait for echo pin to go low
    while (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2));

    // Stop Timer1A
    TimerDisable(TIMER1_BASE, TIMER_A);

    // Read the pulse width from Timer1A
    pulseWidth = TimerValueGet(TIMER1_BASE, TIMER_A);

    // Convert pulse width to distance (in centimeters, assuming speed of sound is 343 m/s)
    // Distance = (pulseWidth * speed of sound) / (2 * Timer1A clock frequency)
    return (int)((pulseWidth * 34300) / (2 * SysCtlClockGet()));
}

