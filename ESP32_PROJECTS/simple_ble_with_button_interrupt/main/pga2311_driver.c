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
/*QueueHandle_t changeAudioQueue;
void volumeControl(void *params){
    uint8_t N;
    while(true){
        if(xQueueReceive(changeAudioQueue, &N, portMAX_DELAY)){
            gpio_set_level(CONFIG_AMP_CS, 0);
            for(int i = 0; i < 16; i++){
                gpio_set_level(CONFIG_AMP_SCLK, 0);
                gpio_set_level(CONFIG_AMP_SDI, 1 & (N >> (7 -(i%8))));
                gpio_set_level(CONFIG_AMP_SCLK, 1);
            }
            gpio_set_level(CONFIG_AMP_CS, 1);
        }
    }
}
*/
#define MINN 140
void amp_init(){

    gpio_reset_pin(CONFIG_AMP_MUTE);
    gpio_set_direction(CONFIG_AMP_MUTE, GPIO_MODE_OUTPUT);
    gpio_reset_pin(CONFIG_AMP_CS);
    gpio_set_direction(CONFIG_AMP_CS, GPIO_MODE_OUTPUT);
    gpio_reset_pin(CONFIG_AMP_SCLK);
    gpio_set_direction(CONFIG_AMP_SCLK, GPIO_MODE_OUTPUT);
    gpio_reset_pin(CONFIG_AMP_SDI);
    gpio_set_direction(CONFIG_AMP_SDI, GPIO_MODE_OUTPUT);

    gpio_set_level(CONFIG_AMP_MUTE, 1);
    gpio_set_level(CONFIG_AMP_CS, 1);
    gpio_set_level(CONFIG_AMP_SCLK, 1);
}

/*
    pga2311 serial gain equation:
    where N is 0-255
    gain(dB) = 31.5 - (0.5(255-N))
    

    volume ranges between 0x00 and 0x7F, aka 0-127
    I2S dac outputs an audio signal in the 10mV range
    aim is to make the gain range between -10dB & 31.5dB
    -10dB = 31.5 - (0.5(255-N))
    N = 175
    min N: 175
    max N: 255
    255-175 80


    if volume is 0, then we want N = 0 aka software mute
*/
void amp_update_volume(uint8_t volume){
    uint8_t N;
    if(volume == 0){
        N = 0;
    }
    else if(volume == 127){
        N = MINN + 80;
    }
    else{
        N = ((uint8_t)((volume / 127.0) * 80)) + MINN;
    }
    printf("Volume: %d N: %d\n", (int)volume, (int)N);
    gpio_set_level(CONFIG_AMP_CS, 0);
        for(int i = 15; i >= 0; i--){
            gpio_set_level(CONFIG_AMP_SCLK, 0);
            gpio_set_level(CONFIG_AMP_SDI, 1 & (N >> (i%8)));
            gpio_set_level(CONFIG_AMP_SCLK, 1);
        }
    gpio_set_level(CONFIG_AMP_CS, 1);
    //xQueueSend(changeAudioQueue, N, portMAX_DELAY);
    
}

void amp_mute(){
    gpio_set_level(CONFIG_AMP_MUTE, 0);
    //amp_update_volume(0);
    return;
}

void amp_unmute(){
    //amp_update_volume(255);
    gpio_set_level(CONFIG_AMP_MUTE, 1);
    return;
}



