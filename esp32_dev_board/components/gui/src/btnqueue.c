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
#include "eeprom.h"
#include "led_pwm.h"
#include "ui.h"
#include "wifi.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
QueueHandle_t btn_queue;
bool wifi_is_connected = false;
const int btn_prov = BTN_PROV;
const int btn_conn = BTN_CONN;
const int btn_back = BTN_BACK;
const int btn_park = BTN_PARK;
const int btn_save = BTN_SAVE;
const int btn_music = BTN_MUSIC;
const int btn_home = BTN_HOME;
const int btn_rblink = BTN_RBLINK;
const int btn_lblink = BTN_LBLINK;
const int btn_light = BTN_LIGHT;
volatile uint8_t lights_mode = 1;
volatile uint8_t light_amount = 0;
volatile uint8_t sound_amount = 0;
static char text_buff[32];

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
            light_amount = lv_slider_get_value(ui_SliderLight);
            sound_amount = lv_slider_get_value(ui_SliderSound);
            eeprom_write(lights_mode, 0x00);
            eeprom_write(light_amount, 0x10);
            eeprom_write(sound_amount, 0x20);
            _ui_screen_change(&ui_HomeScreeen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0,
                              &ui_HomeScreeen_screen_init);
            break;

        case BTN_MUSIC :
            xQueueSend(music_queue, &music_command, 0);
            break;
        case BTN_PARK :
            _ui_screen_change(&ui_ParkMode, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0,
                              &ui_ParkMode_screen_init);
            int i = 1;
            xQueueSend(buzzer_queue, &i, 0);
            break;
        case BTN_HOME :
            _ui_screen_change(&ui_HomeScreeen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0,
                              &ui_HomeScreeen_screen_init);
            i = 2;
            xQueueSend(buzzer_queue, &i, 0);
            break;
        case BTN_RBLINK :
            // upali ledicu na 3sec
            static bool r_blinking = false;
            ESP_LOGI("BTN_QUEUE", "INSIDE RIGHT BLINK");
            if (!r_blinking) {
                r_blinking = true;
                ESP_LOGI("BTN_QUEUE", "STARTING RIGHT BLINK");
                led_blinking(LED_ID_RBLINK, 400, true);
            } else {
                r_blinking = false;
                led_blinking(LED_ID_RBLINK, 400, false);
            }
            break;
        case BTN_LBLINK :
            // upali ledicu na 3sec
            static bool l_blinking = false;
            ESP_LOGI("BTN_QUEUE", "INSIDE LEFT BLINK");
            if (!l_blinking) {
                l_blinking = true;
                ESP_LOGI("BTN_QUEUE", "STARTING LEFT BLINK");
                led_blinking(LED_ID_LBLINK, 400, true);
            } else {
                l_blinking = false;
                led_blinking(LED_ID_LBLINK, 400, false);
            }
            break;
        case BTN_LIGHT :
            if (lights_mode == 1) {
                lights_mode = 2;
                snprintf(text_buff, sizeof(text_buff), "ON");
                lv_label_set_text(ui_LeftSignalizationLabel1, text_buff);
                light_amount = lv_slider_get_value(ui_SliderLight);
                led_on_pwm(LED_ID_BLUE, light_amount);
            } else if (lights_mode == 2) {
                lights_mode = 0;
                snprintf(text_buff, sizeof(text_buff), "OFF");
                lv_label_set_text(ui_LeftSignalizationLabel1, text_buff);
                led_off(LED_ID_BLUE);
            } else if (lights_mode == 0) {
                lights_mode = 1;
                snprintf(text_buff, sizeof(text_buff), "AUTO");
                lv_label_set_text(ui_LeftSignalizationLabel1, text_buff);
                light_amount = lv_slider_get_value(ui_SliderLight);
            }
            break;
        default :
            ESP_LOGW(TAG, "Unknown button pressed: %d", pressed_button);
            break;
        }
    }
}
