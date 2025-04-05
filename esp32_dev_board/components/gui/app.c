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
#include <math.h>
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
#include "i2s.h"
#include "joy.h"
#include "light.h"
#include "temphum.h"
#include "ultrasonic.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
#define GRAVITY_1G     16384.0f // Adjust based on your sensor's 1g value
#define TILT_THRESHOLD 60 // Treshhold when more than 60degrees it's dangerous
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
static void _app_task(void *p_parameter);
static void leds_init();
static void _i2c_read_task(void *p_parameter);
static void _accelerometer_task(void *p_parameter);
static void _time_task(void *p_parameter);
static void _ultrasonic_task(void *p_parameter);
/*--------------------------- VARIABLES --------------------------------------*/
static const char *TAG = "app";
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
static void _app_task(void *p_parameter)
{
    (void)p_parameter;
    // led_on_pwm(LED_ID_BLUE, 20);
    // led_on_pwm(LED_ID_BUZZER, 50);
    // printf("Wifi initalized, starting to provision\n");
    // wifi_provision(); testirano i radi provision
    //  wifi_connect(); // I ovo testirano i radi
    // create accelerometer task

    // APP TASK CE BITI JEDINI TASK KOJI OVDJE RADI SVE NA LOOPU (ZA SADA)
    for (;;) {
        // i ova funkcija testirana i radi
        // get_current_time(strftime_buf, &curr_time);
        // printf("Hello world, current time is: %ld = %s\n", curr_time,
        //      strftime_buf);
        // printf("Hello world, current time is: %ld = %s\n", curr_time,
        //      strftime_buf);
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        // play_audio();
        // led_off(LED_ID_BUZZER);
        // led_toggle(LED_ID_GREEN);
        // led_toggle(LED_ID_RED);
    }
}

static void _ultrasonic_task(void *p_parameter) {}

static void _time_task(void *p_parameter)
{
    (void)p_parameter;
    char strftime_buf[64];
    uint32_t curr_time = 0;
    while (true) {
        get_current_time(strftime_buf, &curr_time);
        ESP_LOGI(TAG, "Current time is: %ld = %s\n", curr_time, strftime_buf);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void _accelerometer_task(void *p_parameter)
{
    (void)p_parameter;
    float fx = 0, fy = 0, fz = 0;

    while (true) {
        fz = acc_get_Z();
        fy = acc_get_Y();
        fx = acc_get_X();

        // Prevent division by zero
        if (fabsf(fy) < 1e-3f) {
            fy = 1e-3f;
        }

        // Calculate tilt
        float tilt_x_deg = atan2f(fx, -fy) * 180.0f / M_PI;
        float tilt_z_deg = atan2f(fz, -fy) * 180.0f / M_PI;

        float tilt_x_pct = (fx / fy) * 100.0f;
        float tilt_z_pct = (fz / fy) * 100.0f;

        // Check for upright orientation
        bool is_upright = true;
        if (tilt_x_deg > TILT_THRESHOLD || tilt_x_deg < -TILT_THRESHOLD
            || tilt_z_deg > TILT_THRESHOLD || tilt_z_deg < -TILT_THRESHOLD) {
            is_upright = false;
        }

        if (!is_upright) {
            // TODO: Add buzzer or LED indication for dangerous tilt
            led_on_pwm(LED_ID_BUZZER, 50); // mozda makni kasnije
        } else {
            led_off(LED_ID_BUZZER); // mozda makni kasnije
        }

        ESP_LOGI(TAG,
                 "X: %f, Y: %f, Z: %f | Tilt X: %.2f° (%.1f%%), Z: %.2f° "
                 "(%.1f%%)%s",
                 fx, fy, fz, tilt_x_deg, tilt_x_pct, tilt_z_deg, tilt_z_pct,
                 is_upright ? "  [UPRIGHT]" : "");

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

static void _i2c_read_task(void *p_parameter)
{
    (void)p_parameter;
    double temp = 0;
    double humidity = 0;
    uint16_t light = 0;
    uint16_t white = 0;
    while (true) {
        temphum_start_measurement();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        temphum_read(&temp, &humidity);
        ESP_LOGI(TAG, "Temperature: %f, Humidity: %f\n", temp, humidity);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        light_read(&light, &white);
        ESP_LOGI(TAG, "Light: %d, White: %d\n", light, white);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
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
    audio_i2s_init();

    wifi_init();
    ultrasonic_init();
    // potencijalno ovo maknuti sve u _app_task() jer app_init()
    // runna na core 1 skupa s GUI, a zelimo da sve ostalo radi na
    // core 0. U testiranju cini se da init moze biti ovjde i da je
    // samo bitno da funkcije connect i/ili provision budu unutar
    // taska na core 0
    xTaskCreatePinnedToCore(_app_task, "app", 4096, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(_i2c_read_task, "i2c_read", 4096, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(_accelerometer_task, "accelerometer", 4096, NULL, 0,
                            NULL, 0);
    xTaskCreatePinnedToCore(_time_task, "time", 4096, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(_ultrasonic_task, "ultrasonic", 4096, NULL, 0, NULL,
                            0);
}
