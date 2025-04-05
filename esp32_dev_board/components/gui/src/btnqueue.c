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
#include "btnqueue.h"

#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <stdio.h>

// squareline
#include "../app.h"
#include "ui.h"
#include "wifi.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
QueueHandle_t btn_queue;
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
void init_button_queue()
{
    (void)p_parameter;
    btn_queue = xQueueCreate(1, sizeof(int));
}
