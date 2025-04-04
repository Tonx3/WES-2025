/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */


#ifndef JOY_H
#define JOY_H

/*--------------------------- INCLUDES ---------------------------------------*/
/*--------------------------- MACROS AND DEFINES -----------------------------*/
#include "adc.h"
#define JOYSTICK_X (34u)
#define JOYSTICK_Y (35u)



#define EXAMPLE_ADC_ATTEN ADC_ATTEN_DB_11

/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
int joy_init(void);
int joy_read_x();
int joy_read_y();
/*--------------------------- EXTERN -----------------------------------------*/
/*--------------------------- GLOBAL FUNCTION PROTOTYPES ---------------------*/

#endif /*JOY_H*/
