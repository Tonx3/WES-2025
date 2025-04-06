// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: hackathon25

#include "../ui.h"

void ui_CarInfo_screen_init(void)
{
    ui_CarInfo = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_CarInfo, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_CarInfo, lv_color_hex(0xA6BDD2), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_CarInfo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_CarInfo, lv_color_hex(0x9BA0D0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_CarInfo, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Speed = lv_label_create(ui_CarInfo);
    lv_obj_set_width(ui_Speed, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Speed, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Speed, -7);
    lv_obj_set_y(ui_Speed, -20);
    lv_obj_set_align(ui_Speed, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Speed, "Speed [m/s]");
    lv_obj_set_style_text_color(ui_Speed, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Speed, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Speed, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Distance_ = lv_label_create(ui_CarInfo);
    lv_obj_set_width(ui_Distance_, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Distance_, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Distance_, -6);
    lv_obj_set_y(ui_Distance_, 15);
    lv_obj_set_align(ui_Distance_, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Distance_, "Distance [m]");
    lv_obj_set_style_text_color(ui_Distance_, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Distance_, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Distance_, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SpeedPlaceholder = lv_label_create(ui_CarInfo);
    lv_obj_set_width(ui_SpeedPlaceholder, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_SpeedPlaceholder, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_SpeedPlaceholder, 0);
    lv_obj_set_y(ui_SpeedPlaceholder, -79);
    lv_obj_set_align(ui_SpeedPlaceholder, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SpeedPlaceholder, "Car Info");
    lv_obj_set_style_text_color(ui_SpeedPlaceholder, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_SpeedPlaceholder, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_SpeedPlaceholder, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Tilt = lv_label_create(ui_CarInfo);
    lv_obj_set_width(ui_Tilt, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Tilt, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Tilt, -96);
    lv_obj_set_y(ui_Tilt, 52);
    lv_obj_set_align(ui_Tilt, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Tilt, "Tilt");
    lv_obj_set_style_text_color(ui_Tilt, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Tilt, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Tilt, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Incline = lv_label_create(ui_CarInfo);
    lv_obj_set_width(ui_Incline, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Incline, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Incline, 88);
    lv_obj_set_y(ui_Incline, 50);
    lv_obj_set_align(ui_Incline, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Incline, "Incline");
    lv_obj_set_style_text_color(ui_Incline, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Incline, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Incline, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_TiltNum = lv_label_create(ui_CarInfo);
    lv_obj_set_width(ui_TiltNum, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_TiltNum, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_TiltNum, -96);
    lv_obj_set_y(ui_TiltNum, 82);
    lv_obj_set_align(ui_TiltNum, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(ui_TiltNum, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_TiltNum, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_TiltNum, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_InclineNum = lv_label_create(ui_CarInfo);
    lv_obj_set_width(ui_InclineNum, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_InclineNum, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_InclineNum, 90);
    lv_obj_set_y(ui_InclineNum, 78);
    lv_obj_set_align(ui_InclineNum, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(ui_InclineNum, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_InclineNum, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_InclineNum, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_CarInfoBackButton = lv_btn_create(ui_CarInfo);
    lv_obj_set_width(ui_CarInfoBackButton, 60);
    lv_obj_set_height(ui_CarInfoBackButton, 50);
    lv_obj_set_x(ui_CarInfoBackButton, -115);
    lv_obj_set_y(ui_CarInfoBackButton, -74);
    lv_obj_set_align(ui_CarInfoBackButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_CarInfoBackButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_CarInfoBackButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_CarInfoBackButton, lv_color_hex(0x5275A1), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_CarInfoBackButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_InfoBackLabel = lv_label_create(ui_CarInfoBackButton);
    lv_obj_set_width(ui_InfoBackLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_InfoBackLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_InfoBackLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_InfoBackLabel, "Home");

    lv_obj_add_event_cb(ui_CarInfoBackButton, ui_event_CarInfoBackButton, LV_EVENT_ALL, NULL);

}
