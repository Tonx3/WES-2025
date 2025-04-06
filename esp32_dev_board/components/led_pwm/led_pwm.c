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
#include "freertos/timers.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
struct blink_info
{
    int led_id;
    int period;
};
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
static void _led_blink_timer_cb_0(TimerHandle_t xTimer);
static void _led_blink_timer_cb_1(TimerHandle_t xTimer);
static void _led_blink_timer_cb_2(TimerHandle_t xTimer);
static void _led_blink_timer_cb_3(TimerHandle_t xTimer);
static void _buzzer_toggle();
/*--------------------------- VARIABLES --------------------------------------*/
bool led_is_on_toggle[4];
TaskHandle_t led_blinking_tasks[4];
BaseType_t timer_0;
BaseType_t timer_1;
BaseType_t timer_2;
BaseType_t timer_3; 
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
void _led_blink_task(void *p_parameter)
{
    int period = ((struct blink_info*)p_parameter)->period;
    int led_id = ((struct blink_info*)p_parameter)->led_id;

    while(true)
    {
        led_toggle(led_id);
        vTaskDelay(period/portTICK_PERIOD_MS);
    }
}

static void _led_blink_timer_cb_0(TimerHandle_t xTimer)
{
    led_toggle(LED_ID_BLUE);
}
static void _led_blink_timer_cb_1(TimerHandle_t xTimer)
{
    led_toggle(LED_ID_GREEN);
}
static void _led_blink_timer_cb_2(TimerHandle_t xTimer)
{
    led_toggle(LED_ID_RED);
}
static void _led_blink_timer_cb_3(TimerHandle_t xTimer)
{
    _buzzer_toggle();
}

static void _buzzer_toggle()
{
    if (led_is_on_toggle[LED_ID_BUZZER]) {
        led_on_pwm(LED_ID_BUZZER, 0);
    } else {
        led_on_pwm(LED_ID_BUZZER, 50);
    }
}
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

void led_blinking(int led_id, int period, bool start)
{
    if(start)
    {
        switch(led_id)
        {
            case 0:
                if(timer_0 != NULL && xTimerIsTimerActive(timer_0) != pdFALSE) xTimerStop(timer_0, (TickType_t)0);
                timer_0 = xTimerCreate("Debounce Timer", pdMS_TO_TICKS(period), pdTRUE, (NULL), &_led_blink_timer_cb_0);
                xTimerStart(timer_0, (TickType_t)0);
            break;
            case 1:
                if(timer_1 != NULL && xTimerIsTimerActive(timer_1) != pdFALSE) xTimerStop(timer_1, (TickType_t)0);
                timer_1 = xTimerCreate("Debounce Timer", pdMS_TO_TICKS(period), pdTRUE, (NULL), &_led_blink_timer_cb_1);
                xTimerStart(timer_1, (TickType_t)0);
            break;
            case 2:
                if(timer_2 != NULL && xTimerIsTimerActive(timer_2) != pdFALSE) xTimerStop(timer_2, (TickType_t)0);
                timer_2 = xTimerCreate("Debounce Timer", pdMS_TO_TICKS(period), pdTRUE, (NULL), &_led_blink_timer_cb_2);
                xTimerStart(timer_2, (TickType_t)0);
            break;
            case 3:
                if(timer_3 != NULL && xTimerIsTimerActive(timer_3) != pdFALSE) xTimerStop(timer_3, (TickType_t)0);
                timer_3 = xTimerCreate("Debounce Timer", pdMS_TO_TICKS(period), pdTRUE, (NULL), &_led_blink_timer_cb_3);
                xTimerStart(timer_3, (TickType_t)0);
            break;
            
        }
    }
    else 
    {
        switch(led_id)
        {
            case 0:
                if(timer_0 != NULL)
                xTimerStop(timer_0, (TickType_t)0);
            break;
            case 1:
                if(timer_1 != NULL)
                xTimerStop(timer_1, (TickType_t)0);
            break;
            case 2:
                if(timer_2 != NULL)
                xTimerStop(timer_2, (TickType_t)0);
            break;
            case 3:
                if(timer_3 != NULL)
                xTimerStop(timer_3, (TickType_t)0);
                led_off(LED_ID_BUZZER);
            break;
            
        }
    }
}
