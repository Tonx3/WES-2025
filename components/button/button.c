/**
 *
 * ░█──░█ ░█▀▀▀ ░█▀▀▀█ ── █▀█ █▀▀█ █▀█ █▀▀
 * ░█░█░█ ░█▀▀▀ ─▀▀▀▄▄ ▀▀ ─▄▀ █▄▀█ ─▄▀ ▀▀▄
 * ░█▄▀▄█ ░█▄▄▄ ░█▄▄▄█ ── █▄▄ █▄▄█ █▄▄ ▄▄▀
 *
 */


/*--------------------------- INCLUDES ---------------------------------------*/
#include "button.h"
//#include "btnqueue.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include <stdio.h>

/*--------------------------- MACROS AND DEFINES -----------------------------*/
#define GPIO_BUTTON_1 (36U)
#define GPIO_BUTTON_2 (32U)
#define GPIO_BUTTON_3 (33U)
#define GPIO_BUTTON_4 (25U)

#define TIMER_PERIOD            (5)
#define NUM_BUTTONS             (4)
#define DEBOUNCE_PAUSE          (50)
#define HOLD_MIN_TIME           (500)
#define MS_TO_TIMER_TICKS(X)    (X/TIMER_PERIOD)

#define DELAY_TIME_MS (1000)

/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
typedef void (*button_pressed_isr_t)(void *p_arg);

/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
static void _btn_1_isr(void *p_arg); // button 1 press
static void _btn_2_isr(void *p_arg); // button 2 press
static void _btn_3_isr(void *p_arg); // button 3 press
static void _btn_4_isr(void *p_arg); // button 4 press

static esp_err_t _button_init(uint8_t pin, button_pressed_isr_t p_isr);
static int button_timer_init();
static void vTimerCallback(TimerHandle_t xTimer);
/*--------------------------- VARIABLES --------------------------------------*/
TimerHandle_t xDebounceTimer;
const int buttonPins[NUM_BUTTONS] = {GPIO_BUTTON_1, GPIO_BUTTON_2, GPIO_BUTTON_3, GPIO_BUTTON_4};
volatile bool buttonIsrCalled[NUM_BUTTONS] = {false, false, false, false};
volatile bool buttonReleased[NUM_BUTTONS] = {0, 0, 0, 0};
volatile int buttonTimerTicksPassed[NUM_BUTTONS] = {0, 0, 0, 0};

/*--------------------------- STATIC FUNCTIONS -------------------------------*/
static void IRAM_ATTR _btn_1_isr(void *p_arg)
{
    (void)p_arg; /* Suppress the unused variable warning*/
    gpio_intr_disable(buttonPins[0]);
    buttonIsrCalled[0] = true;
    buttonReleased[0] = false;
    
}

static void IRAM_ATTR _btn_2_isr(void *p_arg)
{
    (void)p_arg; /* Suppress the unused variable warning*/
    gpio_intr_disable(buttonPins[1]);
    buttonIsrCalled[1] = true;
    buttonReleased[1] = false;
}

static void IRAM_ATTR _btn_3_isr(void *p_arg)
{
    (void)p_arg; /* Suppress the unused variable warning*/
    gpio_intr_disable(buttonPins[2]);
    buttonIsrCalled[2] = true;
    buttonReleased[2] = false;
}

static void IRAM_ATTR _btn_4_isr(void *p_arg)
{
    (void)p_arg; /* Suppress the unused variable warning*/
    gpio_intr_disable(buttonPins[3]);
    buttonIsrCalled[3] = true;
    buttonReleased[3] = false;
}

static int button_timer_init()
{
    xDebounceTimer = xTimerCreate("Debounce Timer", pdMS_TO_TICKS(TIMER_PERIOD), pdTRUE, (NULL), &vTimerCallback);
    return 0;
}

static esp_err_t _button_init(uint8_t pin, button_pressed_isr_t p_isr)
{
    // Configure the GPIO.
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = (GPIO_INTR_POSEDGE),
    };

    esp_err_t esp_err = gpio_config(&io_conf);

    if (ESP_OK == esp_err) {
        // Change gpio interrupt type for a pin.
        esp_err = gpio_set_intr_type(io_conf.pin_bit_mask, io_conf.intr_type);
    }

    if (ESP_OK == esp_err) {
        // Install gpio isr service.
        gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3);
        /* esp_err is not assigned to it because it returns invalid
        statuses when called many times (for many buttons)*/
    }

    if (ESP_OK == esp_err) {
        // Hook isr handler for specific gpio pin.
        esp_err = gpio_isr_handler_add(pin, p_isr, (void *)&pin);
    }

    return esp_err;
}

static void vTimerCallback(TimerHandle_t xTimer)
{
    for(int i = 0; i <NUM_BUTTONS; ++i)
    {
        if(buttonIsrCalled[i]) 
        {
            bool buttonPressed = gpio_get_level(buttonPins[i]);
            if(buttonPressed && ++buttonTimerTicksPassed[i] == MS_TO_TIMER_TICKS(DEBOUNCE_PAUSE))
            {
                // Button is pressed
            }
            else if(buttonPressed && ++buttonTimerTicksPassed[i] > MS_TO_TIMER_TICKS(HOLD_MIN_TIME))
            {
                // Button is helled
            }
            else if(!buttonPressed && ++buttonTimerTicksPassed[i] > MS_TO_TIMER_TICKS(DEBOUNCE_PAUSE))
            {
                // Button is released
                buttonReleased[i]=true;
                buttonTimerTicksPassed[i] = 0;
                buttonIsrCalled[i] = false;
                gpio_intr_enable(buttonPins[i]);
            }
        }
    }
}
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/

int button_init()
{
    _button_init(GPIO_BUTTON_1, _btn_1_isr);
    _button_init(GPIO_BUTTON_2, _btn_2_isr);
    _button_init(GPIO_BUTTON_3, _btn_3_isr);
    _button_init(GPIO_BUTTON_4, _btn_4_isr);
    button_timer_init();
    return 0;
}
