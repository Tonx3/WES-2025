/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
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
#define READ_BIT 1
#define WRITE_BIT 0
#define TX_BUFF_SIZE 2
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
void acc_set_transmit_buffer(int address, int data, int rw_bit);
/*--------------------------- VARIABLES --------------------------------------*/
char transmit_buffer[TX_BUFF_SIZE];
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
void acc_set_transmit_buffer(int address, int data, int rw_bit)
{
    transmit_buffer[0] = ((rw_bit & 0x01) << 7) | (0 << 6) | (address & 0x3F);
    transmit_buffer[1] = data & 0xFF;
}
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
void acc_init()
{
    SPI_init();

    acc_set_transmit_buffer(0x20, 0b01110111, WRITE_BIT);
    SPI_write_read(transmit_buffer, TX_BUFF_SIZE, NULL, 0);
    acc_set_transmit_buffer(0x23, 0b00110000, WRITE_BIT);
    SPI_write_read(transmit_buffer, TX_BUFF_SIZE, NULL, 0);
}

float acc_get_Z()
{ // get X/Y/Z functions return values in g
    char Z_L[2], Z_H[2];
    memset(Z_L, 0, 2);
    memset(Z_H, 0, 2);
    int address = 0x2C;
    acc_set_transmit_buffer(address, 0x00, READ_BIT);
    SPI_write_read(transmit_buffer, 2, Z_L, 2);
    
    address = 0x2D;
    acc_set_transmit_buffer(address, 0x00, READ_BIT);
    SPI_write_read(transmit_buffer, 2, Z_H, 2);
    int16_t Zi = (Z_H[1] << 8) | Z_L[1];
    Zi >>= 6;
    float Z = ((float)Zi) * 48 / 1000;
    return Z;
}

float acc_get_Y()
{
    char Y_L[2], Y_H[2];
    memset(Y_L, 0, 2);
    memset(Y_H, 0, 2);

    int address = 0x2A;
    acc_set_transmit_buffer(address, 0x00, READ_BIT);
    SPI_write_read(transmit_buffer, 2, Y_L, 2);
    
    address = 0x2B;
    acc_set_transmit_buffer(address, 0x00, READ_BIT);
    SPI_write_read(transmit_buffer, 2, Y_H, 2);
 
    int16_t Yi = (Y_H[1] << 8) | Y_L[1];
    Yi >>= 6;
    float Y = ((float)Yi) * 48 / 1000;
    return Y;
}

float acc_get_X()
{
    SPI_set_cs(false);
    char X_L[2], X_H[2];
    memset(X_L, 0, 2);
    memset(X_H, 0, 2);

    int address = 0x28;
    acc_set_transmit_buffer(address, 0x00, READ_BIT);
    SPI_write_read(transmit_buffer, 2, X_L, 2);
    //ESP_LOGI("SPI", "OUT FIRST");

    address = 0x29;
    acc_set_transmit_buffer(address, 0x00, READ_BIT);
    SPI_write_read(transmit_buffer, 2, X_H, 2);

    int16_t Xi = (X_H[1] << 8) | X_L[1];
    Xi >>= 6;
    float X = ((float)Xi) * 48 / 1000;
    
    SPI_set_cs(true);
    return X;
}

void acc_enable_read(bool enable)
{
    // The communication is done when chip select is low
    // So if enable is true, we set chip select to false == 0
    SPI_set_cs(!enable);
}

int dummy_read()
{ // returns 0b00110011 as a set value to check if the reading is correct
    SPI_set_cs(false);
    char address = 0x0F;
    char whoamI[2];
    acc_set_transmit_buffer(address, 0x00, READ_BIT);
    SPI_write_read(transmit_buffer, 2, whoamI, 2);

    return whoamI[1];
}

bool earthquake()
{
    return true;
}
