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
    BTN_UP,
    BTN_RIGHT,
    BTN_DOWN,
    BTN_LEFT,
    BTN_PROV,
    BTN_CONN,
    BTN_PLAY,
    BTN_BACK,
};
/*--------------------------- EXTERN -----------------------------------------*/
extern QueueHandle_t btn_queue;
/*--------------------------- GLOBAL FUNCTION PROTOTYPES ---------------------*/
void init_button_queue();

#endif /*INC_BTNQUEUE_H*/
