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

#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <stdbool.h>
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
bool wifi_is_connected = false;
const int btn_up = BTN_UP;
const int btn_right = BTN_RIGHT;
const int btn_down = BTN_DOWN;
const int btn_left = BTN_LEFT;
const int btn_prov = BTN_PROV;
const int btn_conn = BTN_CONN;
const int btn_back = BTN_BACK;
const int btn_park = BTN_PARK;
const int btn_save = BTN_SAVE;
const int btn_music = BTN_MUSIC;

static const char *TAG = "btnqueue";
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
void init_button_queue()
{
    btn_queue = xQueueCreate(1, sizeof(int));
    if (btn_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create button queue");
        return;
    }
}

void _button_task(void *p_parameter)
{
    (void)p_parameter;
    init_button_queue();
    int pressed_button = 0;
    // task that waits for button press and sends it to the queue
    for (;;) {
        xQueueReceive(btn_queue, &pressed_button, portMAX_DELAY);
        switch (pressed_button) {
        case BTN_UP :
            // TODO: handle up
            break;

        case BTN_RIGHT :
            // TODO: handle right
            break;

        case BTN_DOWN :
            // TODO: handle down
            break;

        case BTN_LEFT :
            // TODO: handle left
            break;

        case BTN_PROV :
            if (!wifi_is_connected) {
                ESP_LOGI(TAG, "WiFi provisioning button pressed");
                // Start WiFi provisioning process
                wifi_provision();
                wifi_is_connected = true;
                lv_obj_set_style_bg_color(ui_Image2, lv_color_hex(0x0000FF),
                                          LV_PART_MAIN | LV_STATE_DEFAULT);
            } else {
                wifi_provision();
                // btn not working if wifi is connected
            }

            break;
        case BTN_CONN :
            if (!wifi_is_connected) {
                ESP_LOGI(TAG, "WiFi provisioning button pressed");
                // Start WiFi provisioning process
                wifi_connect();
                wifi_is_connected = true;
                lv_obj_set_style_bg_color(ui_Image2, lv_color_hex(0x0000FF),
                                          LV_PART_MAIN | LV_STATE_DEFAULT);
            } else {
                // btn not working if wifi is connected
            }
            break;

        case BTN_BACK :

            break;

        case BTN_SAVE :

            break;

        case BTN_MUSIC :
            xQueueSend(music_queue, &music_command, 0);
            break;

        default :
            ESP_LOGW(TAG, "Unknown button pressed: %d", pressed_button);
            break;
        }
    }
}
