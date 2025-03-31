/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */

/*--------------------------- INCLUDES ---------------------------------------*/
#include "led_pwm.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
bool led_is_on_toggle[4];
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
int led_init(int led_id)
{
    int gpio_pin = 0;
    if (led_id == LEDC_CHANNEL_BLUE) {
        gpio_pin = GPIO_LED_BLUE;
    } else if (led_id == LEDC_CHANNEL_GREEN) {
        gpio_pin = GPIO_LED_GREEN;
    } else if (led_id == LEDC_CHANNEL_RED) {
        gpio_pin = GPIO_LED_RED;
    } else if (led_id == LEDC_CHANNEL_BUZZER) {
        gpio_pin = GPIO_BUZZER;
    }

    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,          // isti
        .timer_num = LEDC_TIMER,          // isti
        .duty_resolution = LEDC_DUTY_RES, // isti
        .freq_hz = LEDC_FREQUENCY,        // Set output frequency at 5 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {.speed_mode = LEDC_MODE,
                                          .channel = led_id,
                                          .timer_sel = LEDC_TIMER,
                                          .intr_type = LEDC_INTR_DISABLE,
                                          .gpio_num = gpio_pin,
                                          .duty = 0, // Set duty to %
                                          .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    return 0;
}

int led_on_pwm(int led_id, int pwm_percentage)
{
    ledc_set_duty(LEDC_MODE, led_id, LEDC_DUTY * pwm_percentage / 100);
    ledc_update_duty(LEDC_MODE, led_id);
    if (pwm_percentage > 0) {
        led_is_on_toggle[led_id] = true;
    } else {
        led_is_on_toggle[led_id] = false;
    }
    return 0;
}

int led_on(int led_id)
{
    led_on_pwm(led_id, 100);
    return 0;
}

int led_off(int led_id)
{
    led_on_pwm(led_id, 0);
    return 0;
}

int led_toggle(int led_id)
{
    if (led_is_on_toggle[led_id]) {
        led_off(led_id);
    } else {
        led_on(led_id);
    }
    return 0;
}
