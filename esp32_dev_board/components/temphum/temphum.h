/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */

#ifndef TEMPHUM_H
#define TEMPHUM_H

/*--------------------------- INCLUDES ---------------------------------------*/
#include "esp_err.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- EXTERN -----------------------------------------*/
/*--------------------------- GLOBAL FUNCTION PROTOTYPES ---------------------*/
int temphum_read(double *temp, double *humidity);
// float read_temp(uint8_t temp);
int temphum_start_measurement();
#endif /*TEMPHUM_H*/
