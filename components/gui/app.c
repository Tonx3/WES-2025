/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */

/*--------------------------- INCLUDES ---------------------------------------*/
#include "app.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

// lvgl
#include "lvgl.h"
#include "lvgl_helpers.h"

// squareline
#include "ui.h"

// drivers
#include "led_pwm.h"
#include "mqtt_driver.h"
#include "wifi.h"
#include "accelerometer.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
static void _app_task(void *p_parameter);
static void leds_init();
static void _accel_read_task(void *p_parameter);

/*--------------------------- VARIABLES --------------------------------------*/
/*--------------------------- STATIC FUNCTIONS -------------------------------*/

static void _accel_read_task(void *p_parameter)
{

    while(true)
    {
        float x = acc_get_X();  
        float y = acc_get_Y();
        float z = acc_get_Z();
        printf("Accelerometer reading... X: %f, Y: %f, Z: %f\n\n", x,y,z);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

static void _app_task(void *p_parameter)
{
    (void)p_parameter;
    led_on_pwm(LED_ID_BLUE, 20);
    // led_on_pwm(LED_ID_BUZZER, 50);
    // printf("Wifi initalized, starting to provision\n");
    // wifi_provision(); testirano i radi provision
    wifi_connect(); // I ovo testirano i radi
    char strftime_buf[64];
    uint32_t curr_time = 0;
    acc_init();
    // create accelerometer task
    xTaskCreate(_accel_read_task, "Accelerometer reading", 4096, NULL, 0, NULL);

    for (;;) {
        // i ova funkcija testirana i radi
        get_current_time(strftime_buf, &curr_time);
        printf("Hello world, current time is: %ld = %s\n", curr_time,
               strftime_buf);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        led_off(LED_ID_BUZZER);
        led_toggle(LED_ID_GREEN);
        led_toggle(LED_ID_RED);
    }
}

static void leds_init()
{
    led_init(LED_ID_BLUE);
    led_init(LED_ID_GREEN);
    led_init(LED_ID_RED);
    led_init(LED_ID_BUZZER);
}

/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/

void app_init(void)
{
    ui_init();
    leds_init();
    wifi_init();
    // potencijalno ovo maknuti sve u _app_task() jer app_init()
    // runna na core 1 skupa s GUI, a zelimo da sve ostalo radi na
    // core 0. U testiranju cini se da init moze biti ovjde i da je
    // samo bitno da funkcije connect i/ili provision budu unutar
    // taska na core 0
    xTaskCreatePinnedToCore(_app_task, "app", 4096 * 2, NULL, 0, NULL, 0);
}
