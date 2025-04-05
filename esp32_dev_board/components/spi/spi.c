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
#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>

#include "driver/gpio.h"

#include "spi.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "hal/spi_types.h"

/*--------------------------- MACROS AND DEFINES -----------------------------*/

// We can define the pins for the spi in advance as the other spi device on esp32
// is used for the LCD screen on this board
#define PIN_NUM_MOSI 23
#define PIN_NUM_MISO 19
#define PIN_NUM_SCK  18
#define PIN_NUM_CS   13
#define PIN_NUM_DISP_CS   0
#define PIN_NUM_TOUCH_CS  0

#define CLK_SPEED_SPI 1000 * 10
#define PARALLEL_LINES 4
#define SPI_BUFF_SIZE 1024
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
static void SPI_sendData(uint8_t bufferSize);
static void SPI_sendReceiveData(uint8_t txBufferSize, uint8_t rxBufferSize);
/*--------------------------- VARIABLES --------------------------------------*/

static spi_device_interface_config_t spi_device_interface_config = {
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .mode = 0,
    .clock_speed_hz = CLK_SPEED_SPI, // 10kHz
    .spics_io_num = PIN_NUM_CS,
    .queue_size = 2, // nebitno :)

};

static spi_device_handle_t spi_device_handle;

char spi_tx_buffer[SPI_BUFF_SIZE];
char spi_rx_buffer[SPI_BUFF_SIZE];
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
static void SPI_sendData(uint8_t bufferSize)
{
    esp_err_t ret;
    spi_transaction_t spi_transaction;
    memset(&spi_transaction, 0, sizeof(spi_transaction));       //Zero out the transaction
    spi_transaction.length = bufferSize*8;                   //Command is 8 bits
    spi_transaction.tx_buffer = spi_tx_buffer;             //The data is the cmd itself
    spi_transaction.rx_buffer=NULL;
    //spi_transaction.user = (void*)0;              //D/C needs to be set to 0

    ret = spi_device_polling_transmit(spi_device_handle, &spi_transaction); //Transmit!
    assert(ret == ESP_OK);          //Should have had no issues.
}

static void SPI_sendReceiveData(uint8_t txBufferSize, uint8_t rxBufferSize)
{
    esp_err_t ret;
    spi_transaction_t spi_transaction;
    memset(&spi_transaction, 0, sizeof(spi_transaction));       //Zero out the transaction
    spi_transaction.length = txBufferSize*8;                   //Command is 8 bits
    spi_transaction.tx_buffer = spi_tx_buffer;             //The data is the cmd itself
    spi_transaction.rx_buffer = spi_rx_buffer;
    spi_transaction.rxlength = rxBufferSize*8;

    ret = spi_device_polling_transmit(spi_device_handle, &spi_transaction); //Transmit!
    assert(ret == ESP_OK);          //Should have had no issues.
}
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
void SPI_init()
{
    static bool init = false;
    esp_err_t ret;
    if(!init)
    {
        init = true;
        ret = spi_bus_add_device(SPI3_HOST, &spi_device_interface_config,
                                    &spi_device_handle);
        ESP_ERROR_CHECK(ret);

        SPI_set_cs(true);
    }
}

uint8_t SPI_transaction(uint8_t address, uint8_t rwb, uint8_t data)
{
    uint8_t transmit_buffer[2];
    memset(&transmit_buffer, 0, sizeof(transmit_buffer));
    transmit_buffer[0] = ((rwb & 0x01) << 7) | (0 << 6) | (address & 0x3F);
    transmit_buffer[1] = data & 0xFF;
    // printf("trans:%d:%d\n", transmit_buffer[0], transmit_buffer[1]);
    uint8_t recieve_buffer[2];
    memset(&recieve_buffer, 0, sizeof(recieve_buffer));
    spi_transaction_t spi_transaction = {//   .flags = SPI_TRANS_USE_RXDATA,
                                         .length = 16,
                                         .rxlength = 16,
                                         .tx_buffer = &transmit_buffer,
                                         .rx_buffer = &recieve_buffer};
    esp_err_t ret;
    ret = spi_device_polling_transmit(spi_device_handle, &spi_transaction);
    ESP_ERROR_CHECK(ret);
    //printf("recv:%d:%d\n", recieve_buffer[0], recieve_buffer[1]);
    uint8_t recieved_data = 0;
    recieved_data = recieve_buffer[1];

    return recieved_data;
}

void SPI_write(char *txBuffer, uint8_t txSize)
{
    //ESP_LOGI("SPI", "ATTEMPTING write");
    assert(txSize<=SPI_BUFF_SIZE);
    memset(spi_tx_buffer, 0, SPI_BUFF_SIZE);
    memcpy(spi_tx_buffer, txBuffer, txSize);
    SPI_sendData(txSize);
    
}

void SPI_write_read(char *txBuffer, uint8_t txSize, char *rxBuffer, uint8_t rxSize)
{
    assert(txSize<=SPI_BUFF_SIZE);
    memset(spi_tx_buffer, 0, SPI_BUFF_SIZE);
    memcpy(spi_tx_buffer, txBuffer, txSize);
    
    SPI_sendReceiveData(txSize, rxSize);
    memcpy(rxBuffer, spi_rx_buffer, rxSize);
}

uint8_t SPI_read(char *rxBuffer, uint8_t rxSize)
{

    spi_transaction_t spi_transaction = {.tx_buffer = NULL,
                                         .rxlength = rxSize*8,
                                        .rx_buffer = rxBuffer};
    esp_err_t ret;
    //ESP_LOGI("SPI", "ATTEMPTING read");
    ret = spi_device_polling_transmit(spi_device_handle, &spi_transaction);
    ESP_ERROR_CHECK(ret);
    return ret;
}

void SPI_set_cs(bool set)
{
    gpio_set_level(PIN_NUM_CS, set);
}

void SPI_set_display_cs(bool set)
{
    gpio_set_level(PIN_NUM_DISP_CS, set);
}

void SPI_set_touch_cs(bool set)
{
    gpio_set_level(PIN_NUM_TOUCH_CS, set);
}