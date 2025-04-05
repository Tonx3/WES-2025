/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */

/*--------------------------- INCLUDES ---------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "esp_log.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <esp_timer.h>

#include "driver/gpio.h"

#include "ultrasonic.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
#define PIN_TRIGGER (26U)
#define PIN_ECHO    (27U)

#define PING_TIMEOUT 6000
#define ROUNDTRIP_M  5800.0f
#define ROUNDTRIP_CM 58

#define LOW_DELAY  4
#define HIGH_DELAY 10

#define MAX_DISTANCE_CM   500 // 5m max
#define SOUND_SPEED_CM_US 0.034

/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
void ultrasonic_init()
{
    // Configure the GPIO.
    gpio_set_direction(PIN_TRIGGER, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_ECHO, GPIO_MODE_INPUT);

    // set trigger level to 0
    gpio_set_level(PIN_TRIGGER, 0);
}

void ultrasonic_measure_raw(uint64_t *time_us)
{
    // Set trigger pin to 1 to send the ultrasound wave from the transmitter
    // Ping: Low for 4 us, then high 10 us
    gpio_set_level(PIN_TRIGGER, 0);
    esp_rom_delay_us(LOW_DELAY);
    gpio_set_level(PIN_TRIGGER, 1);
    esp_rom_delay_us(HIGH_DELAY);
    gpio_set_level(PIN_TRIGGER, 0);

    // Start the timer
    uint64_t start_time = esp_timer_get_time();
    // Wait for echo
    while (!gpio_get_level(PIN_ECHO)) {}

    // got echo, measuring
    int64_t echo_start = esp_timer_get_time();
    int64_t time = echo_start;

    while (gpio_get_level(PIN_ECHO)) {
        time = esp_timer_get_time();
        // Break if too much time has passed
    }

    *time_us = time - echo_start;

    return;
}

void ultrasonic_measure(double *distance)
{
    uint64_t time_us;
    ultrasonic_measure_raw(&time_us);
    *distance = (double)time_us / ROUNDTRIP_M;

    // distance_cm = (duration * 0.034) / 2;
    //*distance_cm = (double)(time_us * SOUND_SPEED_CM_US) / 2.0;
    return;
}

void ultrasonic_measure_cm(double *distance)
{
    uint64_t time_us;
    ultrasonic_measure_raw(&time_us);
    *distance = (double)time_us / ROUNDTRIP_CM;

    return;
}
