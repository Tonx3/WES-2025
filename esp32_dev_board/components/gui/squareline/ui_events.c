// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: hackathon25

#include "btnqueue.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "ui.h"

void ParkButtonCb(lv_event_t *e)
{
    xQueueSendFromISR(btn_queue, &btn_park, pdFALSE);
}

void WifiConnectCb(lv_event_t *e)
{
    xQueueSendFromISR(btn_queue, &btn_conn, pdFALSE);
}

void WifiProvisionCb(lv_event_t *e)
{
    xQueueSendFromISR(btn_queue, &btn_prov, pdFALSE);
}

void MusicButtonCb(lv_event_t *e)
{
    xQueueSendFromISR(btn_queue, &btn_music, pdFALSE);
}

void SaveButtonCb(lv_event_t *e)
{
    xQueueSendFromISR(btn_queue, &btn_save, pdFALSE);
}

void HomeParkCb(lv_event_t *e)
{
    xQueueSendFromISR(btn_queue, &btn_home, pdFALSE);
}

void LeftSignalCb(lv_event_t * e)
{
    xQueueSendFromISR(btn_queue, &btn_home, pdFALSE);
}

void RightSignalCb(lv_event_t * e)
{
    xQueueSendFromISR(btn_queue, &btn_home, pdFALSE);
}

void autoLightsCb(lv_event_t * e)
{
	// Your code here
}
