/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */

/*--------------------------- INCLUDES ---------------------------------------*/
#include "driver/dac.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
static const char *TAG = "i2s";
#define SAMPLE_RATE_HZ 16000
extern const unsigned char song_raw[];
extern const unsigned int song_raw_len;
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
void audio_i2s_init()
{
    esp_err_t err = dac_output_enable(DAC_CHANNEL_1);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "DAC enable failed: %s", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "DAC initialized on GPIO25");
    }
}

void play_audio()
{
    const int delay_us = 1000000 / SAMPLE_RATE_HZ; // 125 µs for 8kHz
    // const TickType_t delay = pdMS_TO_TICKS(1000 / SAMPLE_RATE_HZ);

    ESP_LOGI(TAG, "Starting playback of %u bytes", (unsigned int)song_raw_len);

    for (size_t i = 0; i < song_raw_len; ++i) {
        dac_output_voltage(DAC_CHANNEL_1, song_raw[i]); // Output byte to DAC
        esp_rom_delay_us(delay_us);
    }

    ESP_LOGI(TAG, "Playback finished");
}
