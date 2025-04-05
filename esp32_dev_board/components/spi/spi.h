/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */

#ifndef SPI_H
#define SPI_H

/*--------------------------- INCLUDES ---------------------------------------*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <stdio.h>

/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- EXTERN -----------------------------------------*/
/*--------------------------- GLOBAL FUNCTION PROTOTYPES ---------------------*/
void SPI_init();
uint8_t SPI_transaction(uint8_t address, uint8_t rwb, uint8_t data);
void SPI_write(char *txBuffer, uint8_t txSize);
void SPI_write_read(char *txBuffer, uint8_t txSize, char *rxBuffer, uint8_t rxSize);
uint8_t SPI_read(char *rxBuffer, uint8_t rxSize);
void SPI_set_cs(bool set);
void SPI_set_display_cs(bool set);
void SPI_set_touch_cs(bool set);
#endif /*SPI_H*/
