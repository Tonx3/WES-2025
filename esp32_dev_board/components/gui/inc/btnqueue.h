/**
 *    ____    __    ____  _______     _______.      ___     ___    ___    _  _
 *    \   \  /  \  /   / |   ____|   /       |     |__ \   / _ \  |__ \  | || |
 *     \   \/    \/   /  |  |__     |   (----` ______ ) | | | | |    ) | | || |_
 *      \            /   |   __|     \   \    |______/ /  | | | |   / /  |__ _|
 *       \    /\    /    |  |____.----)   |         / /_  | |_| |  / /_     | |
 *        \__/  \__/     |_______|_______/         |____|  \___/  |____|    |_|
 *
 */

#ifndef INC_BTNQUEUE_H
#define INC_BTNQUEUE_H

/*--------------------------- INCLUDES ---------------------------------------*/
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
enum all_btns {
    BTN_PROV,  // WiFi provisioning
    BTN_CONN,  // WiFi connect
    BTN_PARK,  // Park mode
    BTN_BACK,  // Back or park
    BTN_SAVE,  // Save button
    BTN_MUSIC, // Music screen / music control
    BTN_HOME,  // GO home from park screen
    BTN_RBLINK,
    BTN_LBLINK,
    BTN_LIGHT,
};
/*--------------------------- EXTERN -----------------------------------------*/
extern QueueHandle_t btn_queue;
extern const int btn_prov;
extern const int btn_conn;
extern const int btn_park;
extern const int btn_back;
extern const int btn_save;
extern const int btn_music;
extern const int btn_home;
extern const int btn_rblink;
extern const int btn_lblink;
extern const int btn_light;
extern volatile uint8_t lights_mode;
extern volatile uint8_t light_amount;
extern volatile uint8_t sound_amount;
/*--------------------------- GLOBAL FUNCTION PROTOTYPES ---------------------*/
void init_button_queue();
void _button_task(void *p_parameter);

#endif /*INC_BTNQUEUE_H*/
