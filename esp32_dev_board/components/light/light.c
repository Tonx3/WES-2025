/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */

/*--------------------------- INCLUDES ---------------------------------------*/
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>

#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "i2c.h"
#include <stdio.h>
/*--------------------------- MACROS AND DEFINES -----------------------------*/
#define LIGHT_ADDR 0x90
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
static const char *TAG = "light";
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
int light_init()
{
    // prvi byte je command code 00,komanda isto sve 0 za default stanje
    uint8_t conf_buff[3] = {0x00, 0x00, 0x00};
    I2C_write(LIGHT_ADDR, conf_buff, sizeof(conf_buff));

    uint8_t command_buff = 0x07;
    uint8_t whoami[2];
    I2C_read_register(LIGHT_ADDR, command_buff, whoami, 2);

    ESP_LOGI(TAG, "inizalized WHOAMI: %02x:%02x", whoami[0], whoami[1]);
    return 0;
}
int light_read(uint16_t *light, uint16_t *white)
{
    uint8_t command_buff = 0x04;
    uint8_t als_buffer[2];
    I2C_read_register(LIGHT_ADDR, command_buff, als_buffer, 2);

    // printf("%x:%x:%x::%x:%x:%x\n", als_buffer[0], als_buffer[1],
    // read_buffer[2], read_buffer[3], read_buffer[4], read_buffer[5]);

    command_buff = 0x05;
    uint8_t white_buffer[2];
    I2C_read_register(LIGHT_ADDR, command_buff, white_buffer, 2);

    *light = (uint16_t)((((uint16_t)als_buffer[0]) << 8) | als_buffer[1]);
    *white = (uint16_t)((((uint16_t)white_buffer[0]) << 8) | white_buffer[1]);

    return 0;
}

int light_configure()
{
    ESP_LOGI(TAG, "Configuring light sensor not implemented");
    return 0;
}
