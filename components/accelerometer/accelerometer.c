/**
 *    ____    __    ____  _______     _______.      ___     ___    ___    _  _
 *    \   \  /  \  /   / |   ____|   /       |     |__ \   / _ \  |__ \  | || |
 *     \   \/    \/   /  |  |__     |   (----` ______ ) | | | | |    ) | | || |_
 *      \            /   |   __|     \   \    |______/ /  | | | |   / /  |__ _|
 *       \    /\    /    |  |____.----)   |         / /_  | |_| |  / /_     | |
 *        \__/  \__/     |_______|_______/         |____|  \___/  |____|    |_|
 *
 */

/*--------------------------- INCLUDES ---------------------------------------*/
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_pwm.h"
#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>

#include "driver/gpio.h"

#include "accelerometer.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "hal/spi_types.h"

/*--------------------------- MACROS AND DEFINES -----------------------------*/
#define PIN_NUM_MOSI 23
#define PIN_NUM_MISO 19
#define PIN_NUM_SCK  18
#define PIN_NUM_CS   13

#define CLK_SPEED_SPI 1000 * 10
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
void acc_init()
{
    SPI_init();
}

float acc_get_Z()
{ // get X/Y/Z functions return values in g
    int8_t Z_L = SPI_read(0x2C);
    int8_t Z_H = SPI_read(0x2D);
    int16_t Zi = (Z_H << 8) | Z_L;
    Zi >>= 6;
    float Z = ((float)Zi) * 48 / 1000;
    return Z;
}

float acc_get_Y()
{
    int8_t Y_L = SPI_read(0x2A);
    int8_t Y_H = SPI_read(0x2B);
    int16_t Yi = (Y_H << 8) | Y_L;
    Yi >>= 6;
    float Y = ((float)Yi) * 48 / 1000;
    return Y;
}

float acc_get_X()
{
    int8_t X_L = SPI_read(0x28);
    int8_t X_H = SPI_read(0x29);
    int16_t Xi = (X_H << 8) | X_L;
    Xi >>= 6;
    float X = ((float)Xi) * 48 / 1000;
    return X;
}

int dummy_read()
{ // returns 0b00110011 as a set value to check if the reading is correct
    uint8_t who = SPI_read(0x0F);
    return who;
}

bool earthquake()
{
    return true;
}
