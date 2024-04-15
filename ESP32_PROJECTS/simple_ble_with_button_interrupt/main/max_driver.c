#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "freertos/xtensa_api.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include <sdkconfig.h>
#include "freertos/queue.h"
void amp_init(){

    gpio_reset_pin(CONFIG_MAX_SDMODE);
    gpio_set_direction(CONFIG_MAX_SDMODE, GPIO_MODE_OUTPUT);

    gpio_set_level(CONFIG_MAX_SDMODE, 0);
}
void amp_update_volume(uint8_t volume){
    if(volume == 0){
        gpio_set_level(CONFIG_MAX_SDMODE, 0);
    }
    else{
        gpio_set_level(CONFIG_MAX_SDMODE, 1);
    }
    
}

void amp_mute(){
    gpio_set_level(CONFIG_MAX_SDMODE, 0);
    return;
}

void amp_unmute(){
    gpio_set_level(CONFIG_MAX_SDMODE, 1);
    return;
}



