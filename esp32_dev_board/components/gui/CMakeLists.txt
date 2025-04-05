idf_component_register(
    SRCS
        "gui.c"
        "app.c"

        "squareline/components/ui_comp_hook.c"

        "squareline/screens/ui_HomeScreeen.c"
        "squareline/screens/ui_CarInfo.c"
        "squareline/screens/ui_ParkMode.c"
        "squareline/screens/ui_Settings.c"

        "squareline/images/ui_img_730112842.c"
        "squareline/images/ui_img_1548857385.c"
        "squareline/images/ui_img_1896724463.c"
        "squareline/images/ui_img_1924809568.c"
        "squareline/images/ui_img_1600593050.c"


        "squareline/ui.c"
        "squareline/ui_helpers.c"
        "squareline/ui_events.c"

    INCLUDE_DIRS
        ""
        "squareline"

    REQUIRES
        lvgl
        lvgl_esp32_drivers
        esp_timer
        led_pwm
        wifi
        mqtt_driver
        button
        accelerometer
        joy
        i2c
        temphum
        light
        ultrasonic
        i2s
)
