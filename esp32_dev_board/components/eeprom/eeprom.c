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
#define MAX_TX_BUFFER (9)
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
uint8_t tx_buffer[MAX_TX_BUFFER];
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
void eeprom_read(uint8_t *data, uint32_t size, uint8_t address)
{
    I2C_read_register(EEPROM_ADDRESS, address, data, size);
}
void eeprom_write(uint8_t *data, uint32_t size, uint8_t address)
{
    assert(size<=8);
    memset(tx_buffer, 0, MAX_TX_BUFFER);
    tx_buffer[0] = address;
    memset(&tx_buffer[1], data, size);
    I2C_write(EEPROM_ADDRESS, tx_buffer, size+1);
}
