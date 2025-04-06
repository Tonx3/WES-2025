/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */

#ifndef GUI_APP_H
#define GUI_APP_H

/*--------------------------- INCLUDES ---------------------------------------*/
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- EXTERN -----------------------------------------*/
extern QueueHandle_t music_queue;
extern QueueHandle_t buzzer_queue;
extern int music_command;
extern int buzzer_activate_command;
extern int buzzer_disable_command;
/*--------------------------- GLOBAL FUNCTION PROTOTYPES ---------------------*/

void app_init(void);

#endif /*GUI_APP_H*/
