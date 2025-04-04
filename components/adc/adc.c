/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */

/*--------------------------- INCLUDES ---------------------------------------*/
#include "adc.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
static int adc_raw[2][10];
adc_oneshot_unit_handle_t adc_handles[2];
static bool initialized[2] = {false, false};
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
int adc_unit_init(uint8_t unit)
{
    if(unit == 0 || unit > 2)
    {
        return -1;
    }
    //-------------ADC1 Init---------------//
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = unit-1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handles[unit-1]));
    initialized[unit-1] = true;

    return 0;
}

int adc_init_channel(uint8_t unit, int channel)
{
    if(unit == 0 || unit > 2)
    {
        return -1;
    }
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = EXAMPLE_ADC_ATTEN,
    };
    if(!initialized[unit-1])
    {
        adc_unit_init(unit);
    }
    adc_oneshot_config_channel(adc_handles[unit-1], channel-1, &config);
    return 0;
}

int adc_read(uint8_t unit, int channel)
{
    int raw_data;
    adc_oneshot_read(adc_handles[unit-1], channel-1, &raw_data);
    printf("Raw data channel: %d, data: %d\n", channel, raw_data);
    return ((raw_data * 100) / 4095);
}