
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_adc/adc_oneshot.h"


//includes for button interrupt
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "ble_handling.c"
#include "pga2311_driver.h"
#define CRASH_INPUT_TIVA CONFIG_CRASH_INPUT

#define LED_PIN 2

#define ADC1_CHAN ADC_CHANNEL_3
#define CRG_BUT_INPUT_PIN CONFIG_CRG_BUTTON
#define CRG_ADC_INPUT_PIN CONFIG_CRG_ADC
#define ADC_ATTEN ADC_ATTEN_DB_6
#define CRG_LED0 CONFIG_CRG_LED0
#define CRG_LED1 CONFIG_CRG_LED1
#define CRG_LED2 CONFIG_CRG_LED2
#define CRG_LED3 CONFIG_CRG_LED3


bool crashed = false;
adc_oneshot_unit_handle_t adc_handle;
int state = 0;
QueueHandle_t crashInterputQueue;
QueueHandle_t altInteruptQueue;


void gpio_init();
void adc_init();

static void IRAM_ATTR gpio_interrupt_handler(void *args)
{
    int pinNumber = (int)args;
    if(pinNumber == CRASH_INPUT_TIVA){
        xQueueSendFromISR(crashInterputQueue, &pinNumber, NULL);
    }
    else if(pinNumber == CRG_BUT_INPUT_PIN){
        xQueueSendFromISR(altInteruptQueue, &pinNumber, NULL);
    }
}

void Crash_Detection(void *params)
{
    int pinNumber, count = 0;
    int level = 0;
    while (true)
    {
        if (xQueueReceive(crashInterputQueue, &pinNumber, portMAX_DELAY)){
            if(pinNumber == CRASH_INPUT_TIVA){
                uint8_t message[] = {'c','r','a','s','h'};
                esp_ble_gatts_set_attr_value(gl_profile_tab[PROFILE_A_APP_ID].char_handle, sizeof(message),message);
                level = 1;
                crashed = true;
            }
            printf("GPIO %d was pressed %d times. crashed is %d\n", pinNumber, count++, crashed);
            gpio_set_level(LED_PIN, level);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        
    }
}
void CRG_Reading(void *params)
{
    /*
    our lipo batter has specs:
    charge voltage: 4.2
    cutoff voltage: 3.0
    using 4 LEDs to represent charge left
    led0 on = (vbat > 3.2) 
    led1 on = (vbat > 3.5)
    led2 on = (vbat > 3.8)
    led3 on = (vbat > 4.1)

    voltage is dropped w/ resistors,
    47k connected to ground
    220k connected to 47k and vbat
    adc measured between the resistors

    arbitrary measurments at atten = 6
    3.2 ~ 1050
    3.5 ~ 1170
    3.8 ~ 1280
    4.1 ~ 1400
    */
    int pinNumber;
    while(true){
        if(xQueueReceive(altInteruptQueue, &pinNumber, portMAX_DELAY))
        {
            int loops = 10;
            int adc_raw;
            int adc_avg = 0;
            for(int i = 0; i < loops; i++){
                adc_oneshot_read(adc_handle, ADC1_CHAN, &adc_raw);
                adc_avg+= adc_raw;
            }
            adc_avg = adc_avg / loops;
            printf("avg adc: %u\n", adc_raw);
            if(adc_avg > 1000){
                gpio_set_level(CRG_LED0, 1);
            }
            if(adc_avg > 1120){
                gpio_set_level(CRG_LED1, 1);
            }
            if(adc_avg > 1240){
                gpio_set_level(CRG_LED2, 1);
            }
            if(adc_avg > 1360){
                gpio_set_level(CRG_LED3, 1);
            }
            vTaskDelay(pdMS_TO_TICKS(2000));
            gpio_set_level(CRG_LED0, 0);
            gpio_set_level(CRG_LED1, 0);
            gpio_set_level(CRG_LED2, 0);
            gpio_set_level(CRG_LED3, 0);

            
        }
        
    }
}

void app_main()
{   
    bt_init();
    gpio_init();
    adc_init();
    amp_init();

    uint8_t message[] = {'n', 'o', 'c','r','a','s','h'};
    esp_ble_gatts_set_attr_value(gl_profile_tab[PROFILE_A_APP_ID].char_handle, sizeof(message),message);
    crashInterputQueue = xQueueCreate(1, sizeof(int));
    xTaskCreate(Crash_Detection, "Crash_Detection", 2048, NULL, 2, NULL);
    altInteruptQueue = xQueueCreate(1, sizeof(int));
    xTaskCreate(CRG_Reading, "Charge_reading", 2048, NULL, 1, NULL);//crg read task has lower priority than crashing

    gpio_install_isr_service(0);
    gpio_isr_handler_add(CRASH_INPUT_TIVA, gpio_interrupt_handler, (void *)CRASH_INPUT_TIVA);
    gpio_isr_handler_add(CRG_BUT_INPUT_PIN, gpio_interrupt_handler, (void*)CRG_BUT_INPUT_PIN);
}

void gpio_init(){
    
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    gpio_reset_pin(CRASH_INPUT_TIVA);
    gpio_set_direction(CRASH_INPUT_TIVA, GPIO_MODE_INPUT);
    gpio_pulldown_en(CRASH_INPUT_TIVA);
    gpio_pullup_dis(CRASH_INPUT_TIVA);
    gpio_set_intr_type(CRASH_INPUT_TIVA, GPIO_INTR_POSEDGE);

    gpio_reset_pin(CRG_BUT_INPUT_PIN);
    gpio_set_direction(CRG_BUT_INPUT_PIN, GPIO_MODE_INPUT);
    gpio_pulldown_en(CRG_BUT_INPUT_PIN);
    gpio_pullup_dis(CRG_BUT_INPUT_PIN);
    gpio_set_intr_type(CRG_BUT_INPUT_PIN, GPIO_INTR_NEGEDGE);

    gpio_reset_pin(CRG_LED0);
    gpio_set_direction(CRG_LED0, GPIO_MODE_OUTPUT);
    gpio_reset_pin(CRG_LED1);
    gpio_set_direction(CRG_LED1, GPIO_MODE_OUTPUT);
    gpio_reset_pin(CRG_LED2);
    gpio_set_direction(CRG_LED2, GPIO_MODE_OUTPUT);
    gpio_reset_pin(CRG_LED3);
    gpio_set_direction(CRG_LED3, GPIO_MODE_OUTPUT);

    
    
}

void adc_init() {
    //adc init
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

    //adc config

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC1_CHAN, &config));
}