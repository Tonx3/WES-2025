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
#include "freertos/queue.h"
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
#include "btnqueue.h"
#include "esp_log.h"
#include "i2c.h"
#include "i2s.h"
#include "infrared.h"
#include "joy.h"
#include "light.h"
#include "myadc.h"
#include "potentiometer.h"
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
static void _dark_task(void *p_parameter);
static void _accelerometer_task(void *p_parameter);
static void _time_task(void *p_parameter);
static void _ultrasonic_task(void *p_parameter);
static void _pot_read_task(void *p_parameter);
static void _ir_read_task(void *p_parameter);
static void _buzzer_task(void *p_parameter);
/*--------------------------- VARIABLES --------------------------------------*/
static const char *TAG = "app";
static char temp_str[32];
static char time_str[32];
static char date_str[32];
static char hum_str[32];
QueueHandle_t music_queue;
QueueHandle_t buzzer_queue;
int music_command = 1;
int buzzer_activate_command = 1;
int buzzer_disable_command = 1;
QueueHandle_t dark_queue;
int dark_command = 1;
int light_command = 2;
static uint8_t lights_mode = 1; // 0 - off, 1 - automatic, 2 - on
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
static void _buzzer_task(void *p_parameter)
{
    (void)p_parameter;
    int received = 0;
    buzzer_queue = xQueueCreate(1, sizeof(int));
    bool buzzer_active = false;
    while (true) {
        xQueueReceive(buzzer_queue, &received, 0);
        if (received == 1) {
            buzzer_active = true;
            ESP_LOGI(TAG, "Buzzer ON\n");
        } else if(received == 2)
        {
            buzzer_active = false;
            ESP_LOGI(TAG, "Buzzer OFF\n");
        } else {
            ESP_LOGI(TAG, "Unknown command\n");
        }

        if(buzzer_active)
        {
            double distance;
            ultrasonic_measure_cm(&distance);
            ESP_LOGI(TAG, "Distance%.2f", distance);
            int percentage = 0;
            if(150 > distance)
            {
                percentage = 50;
            }
            led_on_pwm(LED_ID_BUZZER, percentage);
        }
        else
        {
            led_on_pwm(LED_ID_BUZZER, 0);
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void _ir_read_task(void *p_parameter)
{
    while (true) {
        int val = ir_read();
        ESP_LOGI(TAG, "IR sensor reading:  %d", val);
        vTaskDelay(3330 / portTICK_PERIOD_MS);
    }
}

static void _pot_read_task(void *p_parameter)
{
    while (true) {
        int val = pot_read();
        ESP_LOGI(TAG, "Potentiometer reading:  %d", val);
        vTaskDelay(3330 / portTICK_PERIOD_MS);
    }
}

static void _ultrasonic_task(void *p_parameter)
{
    while (true) {
        double distance;
        ultrasonic_measure_cm(&distance);
        ESP_LOGI(TAG, "Ultrasonic reading... distance: %lf cm\n\n", distance);
        vTaskDelay(3330 / portTICK_PERIOD_MS);
    }
}

static void _app_task(void *p_parameter)
{
    (void)p_parameter;
    // APP TASK CE BITI JEDINI TASK KOJI OVDJE RADI SVE NA LOOPU (ZA SADA)
    for (;;) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void _time_task(void *p_parameter)
{
    (void)p_parameter;
    char strftime_buf[64];
    uint32_t curr_time = 0;
    while (true) {
        // Thu Jan  1 00:00:08 1970
        get_current_time(strftime_buf, &curr_time);
        ESP_LOGI(TAG, "Current time is: %ld = %s\n", curr_time, strftime_buf);
        strncpy(time_str, &strftime_buf[11], 5); // copy "00:00"
        time_str[8] = '\0';
        lv_label_set_text(ui_TimeLabel, time_str);
        // Extract date "Thu Jan  1" and add space + year
        strncpy(date_str, &strftime_buf[0], 10);
        date_str[10] = '\0';
        strncat(date_str, " ", sizeof(date_str) - strlen(date_str) - 1);
        strncat(date_str, &strftime_buf[20], 4); // Append "1970"
        lv_label_set_text(ui_DateLabel, date_str);
        vTaskDelay(30000 / portTICK_PERIOD_MS);
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
        snprintf(temp_str, sizeof(temp_str), "%.2f°C", temp);
        lv_label_set_text(ui_TempLabel, temp_str);
        snprintf(hum_str, sizeof(hum_str), "%.2f%%", humidity);
        lv_label_set_text(ui_HumidityLabel, hum_str);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
        light_read(&light, &white);
        if (light > 4000) {
            xQueueSend(dark_queue, &light_command, 0);
        } else {
            xQueueSend(dark_queue, &dark_command, 0);
        }
        ESP_LOGI(TAG, "Light: %d, White: %d\n", light, white);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void _dark_task(void *p_parameter)
{
    (void)p_parameter;
    int recived = 0;
    dark_queue = xQueueCreate(1, sizeof(int));
    while (true) {
        xQueueReceive(dark_queue, &recived, portMAX_DELAY);
        if (recived == 1) {
            // dark mode stuff
            lv_obj_set_style_bg_color(ui_HomeScreeen, lv_color_hex(0x360F5F),
                                      LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_CarInfo, lv_color_hex(0x360F5F),
                                      LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_Settings, lv_color_hex(0x360F5F),
                                      LV_PART_MAIN | LV_STATE_DEFAULT);
            ESP_LOGI(TAG, "Dark mode activated\n");
            if (lights_mode == 1) {
                // UPALI LEDICE
            }
        } else if (recived == 2) {
            // light mode stuff
            lv_obj_set_style_bg_color(ui_HomeScreeen, lv_color_hex(0xA6BDD2),
                                      LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_CarInfo, lv_color_hex(0xA6BDD2),
                                      LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_Settings, lv_color_hex(0xA6BDD2),
                                      LV_PART_MAIN | LV_STATE_DEFAULT);
            ESP_LOGI(TAG, "Light mode activated\n");
            if (lights_mode == 1) {
                // GASI LEDICE LEDICE
            }
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

static void _music_task(void *p_parameter)
{
    (void)p_parameter;
    int recived = 0;
    music_queue = xQueueCreate(1, sizeof(int));
    while (true) {
        xQueueReceive(music_queue, &recived, portMAX_DELAY);
        if (recived == 1) {
            ESP_LOGI(TAG, "Music play\n");
            play_audio();
        } else {
            ESP_LOGI(TAG, "Unknown command\n");
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

static void leds_init()
{
    led_init(LED_ID_BLUE);
    // led_init(LED_ID_GREEN);
    // led_init(LED_ID_RED);
    led_init(LED_ID_BUZZER);
}
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/

void app_init(void)
{
    ui_init();
    leds_init();
    // acc_init();

    if (pot_init() != 0) {
        ESP_LOGE(TAG, "Unable to init adc pot");
        return;
    }
    if (ir_init() != 0) {
        ESP_LOGE(TAG, "Unable to init adc ir");
        return;
    }

    //  int acc_whoami = dummy_read();
    //  ESP_LOGI("TAG", "Acc who am I: 0x%x", acc_whoami);
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
    //  xTaskCreatePinnedToCore(_accelerometer_task, "accelerometer", 4096,
    //  NULL, 0,
    //                         NULL, 0);
    xTaskCreatePinnedToCore(_time_task, "time", 4096, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(_ultrasonic_task, "ultrasonic", 4096, NULL, 0, NULL,
                            0);
    xTaskCreatePinnedToCore(_pot_read_task, "pot", 4096, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(_ir_read_task, "ir", 4096, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(_button_task, "button", 4096, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(_music_task, "music", 4096, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(_dark_task, "dark", 4096, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(_buzzer_task, "buzzer", 4096, NULL, 0, NULL, 0);
}
