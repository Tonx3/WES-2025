/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */
#ifndef ADC_H
#define ADC_H

/*--------------------------- INCLUDES ---------------------------------------*/
#include <stdlib.h>
/*--------------------------- MACROS AND DEFINES -----------------------------*/

#define EXAMPLE_ADC_ATTEN ADC_ATTEN_DB_12

/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
int adc_init_unit(uint8_t unit);
int adc_init_channel(uint8_t unit, int channel);
int adc_read(uint8_t unit, int channel);
/*--------------------------- EXTERN -----------------------------------------*/
/*--------------------------- GLOBAL FUNCTION PROTOTYPES ---------------------*/

#endif /*ADC_H*/
