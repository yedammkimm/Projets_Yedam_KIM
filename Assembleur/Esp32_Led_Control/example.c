#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "example";

static uint8_t s_led_state = 0;

static led_strip_handle_t led_strip;

static void blink_led(int duration)
{
    led_strip_set_pixel(led_strip, 0, 16, 16, 16);
    led_strip_refresh(led_strip);
    vTaskDelay(duration / portTICK_PERIOD_MS);
    led_strip_clear(led_strip);
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = 8,
        .max_leds = 1,
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}

int increment( int i );

int add(int a, int b);

int fib(int a, int b);

void main(int N, int* tab);


void app_main(void)
{
    int d;
    int N = 10; 
    int tab[] = {12,36,8,5,59,798,65,48,12,58};
    main(N,tab);
    /*Configure the peripheral according to the LED type */
    configure_led();
    while (1) {
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        for (d=0; d < N ; d++) {
            ESP_LOGI(TAG, "a = %d", tab[d]);
            blink_led(500);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
         
    }
}


/*
void app_main(void)
{
    int fib1,fib2,next,d;
    fib1=0;
    fib2=1;
     Configure the peripheral according to the LED type 
    configure_led();
    while (1) {
        next = fib2;
        fib2 = fib(fib1,next);
        fib1 = next;
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        ESP_LOGI(TAG, "fib1 = %d", fib1);
        for (d=0; d < next ; d++) {
            blink_led(500);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
         
    }
}
*/

/*
exercice 1
void app_main(void)
{
    int a,b,c,d;
    a=7;
    b=7;

     Configure the peripheral according to the LED type 
    configure_led();

    while (1) {
        c = add(a,b);
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        ESP_LOGI(TAG, "a = %d", a);
        for (d=0; d < c; d++) {
            blink_led(500);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
*/
