/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */

#ifndef EEPROM_H
#define EEPROM_H

/*--------------------------- INCLUDES ---------------------------------------*/
#include "esp_err.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- EXTERN -----------------------------------------*/
/*--------------------------- GLOBAL FUNCTION PROTOTYPES ---------------------*/
void eeprom_read(uint8_t *data, uint32_t size, uint8_t address);
void eeprom_write(uint8_t *data, uint32_t size, uint8_t address);

#endif /*I2C_H*/
