#ifndef ULTRASONIC_DRIVER_H_
#define ULTRASONIC_DRIVER_H_

#include "inc/hw_memmap.h"                  // Macros defining the memory map of the Tiva C Series device
#include "inc/hw_types.h"                   // Defines common types and macros
#include "inc/hw_timer.h"                   // Defines and macros used when accessing the timer
#include "inc/hw_gpio.h"

void ultrasonic_init(void);
void ultrasonicTimer_init(void);
void ultrasonic_trigger(void);
uint32_t ultrasonic_get(void);




#endif
