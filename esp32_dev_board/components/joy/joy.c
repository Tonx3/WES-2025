/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */


/*--------------------------- INCLUDES ---------------------------------------*/
#include "joy.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*--------------------------- MACROS AND DEFINES -----------------------------*/

#define JOY_ADC_UNIT        (1)
#define JOY_ADC_CHANNEL_X   (6)
#define JOY_ADC_CHANNEL_Y   (7)

/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
int joy_init(void)
{
    // Joypad is on adc unit 1, channels 6 and 7
    int ret1 = adc_init_channel(JOY_ADC_UNIT, JOY_ADC_CHANNEL_X);
    int ret2 = adc_init_channel(JOY_ADC_UNIT, JOY_ADC_CHANNEL_Y);
    return ret1 || ret2;
}

int joy_read_x()
{
    return adc_read(JOY_ADC_UNIT, JOY_ADC_CHANNEL_X);
}

int joy_read_y()
{
    return adc_read(JOY_ADC_UNIT, JOY_ADC_CHANNEL_Y);
}
