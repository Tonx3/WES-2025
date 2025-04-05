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
#include "accelerometer.h"
#include "led_pwm.h"
#include "mqtt_driver.h"
#include "wifi.h"
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"
#include "i2c.h"
#include "joy.h"
#include "light.h"
#include "temphum.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
static void _app_task(void *p_parameter);
static void leds_init();
static void _accel_read_task(void *p_parameter);

/*--------------------------- VARIABLES --------------------------------------*/
static const char *TAG = "app";
/*--------------------------- STATIC FUNCTIONS -------------------------------*/

static void _accel_read_task(void *p_parameter)
{
    if (joy_init() != 0) {
        ESP_LOGE(TAG, "Unable to init adc");
        return;
    }

    ESP_LOGI(TAG, "Task entered");
    while (true) {
        float x_pos = joy_read_x();
        float y_pos = joy_read_y();
        //   ESP_LOGI(TAG, "Joy reading... X: %f, Y: %f\n\n", x_pos, y_pos);
        vTaskDelay(333 / portTICK_PERIOD_MS);
    }
}

static void _app_task(void *p_parameter)
{
    (void)p_parameter;
    led_on_pwm(LED_ID_BLUE, 20);
    // led_on_pwm(LED_ID_BUZZER, 50);
    // printf("Wifi initalized, starting to provision\n");
    // wifi_provision(); testirano i radi provision
    // wifi_connect(); // I ovo testirano i radi
    char strftime_buf[64];
    uint32_t curr_time = 0;
    // create accelerometer task
    xTaskCreate(_accel_read_task, "Accelerometer reading", 4096, NULL, 0, NULL);
    double temp = 0;
    double humidity = 0;
    uint16_t light = 0;
    uint16_t white = 0;
    for (;;) {
        // i ova funkcija testirana i radi
        get_current_time(strftime_buf, &curr_time);
        // printf("Hello world, current time is: %ld = %s\n", curr_time,
        //      strftime_buf);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        led_off(LED_ID_BUZZER);
        led_toggle(LED_ID_GREEN);
        led_toggle(LED_ID_RED);
        temphum_start_measurement();
        light_read(&light, &white);
        ESP_LOGI(TAG, "Light: %d, White: %d\n", light, white);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        temphum_read(&temp, &humidity);
        ESP_LOGI(TAG, "Temperature: %f, Humidity: %f\n", temp, humidity);
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
    acc_init();
    int acc_whoami = dummy_read();
    ESP_LOGI("TAG", "Acc who am I: 0x%x", acc_whoami);
    if (joy_init() != 0) {
        ESP_LOGE(TAG, "Unable to init adc");
        return;
    }
    I2C_init();
    light_init();

    wifi_init();
    // potencijalno ovo maknuti sve u _app_task() jer app_init()
    // runna na core 1 skupa s GUI, a zelimo da sve ostalo radi na
    // core 0. U testiranju cini se da init moze biti ovjde i da je
    // samo bitno da funkcije connect i/ili provision budu unutar
    // taska na core 0
    xTaskCreatePinnedToCore(_app_task, "app", 4096 * 2, NULL, 0, NULL, 0);
}
