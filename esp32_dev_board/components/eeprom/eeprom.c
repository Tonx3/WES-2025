/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */

/*--------------------------- INCLUDES ---------------------------------------*/

#include "eeprom.h"
#include "i2c.h"
#include "string.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
#define EEPROM_ADDRESS (0x50)
#define MAX_TX_BUFFER  (9)
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
uint8_t tx_buffer[MAX_TX_BUFFER];
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
void eeprom_read(uint8_t *data, uint8_t address)
{
    I2C_write(EEPROM_ADDRESS, &address, 1);
    I2C_read(EEPROM_ADDRESS, data, 1);
}
void eeprom_write(uint8_t data, uint8_t address)
{
    tx_buffer[0] = address;
    tx_buffer[1] = data;
    I2C_write(EEPROM_ADDRESS, tx_buffer, 2);
}
