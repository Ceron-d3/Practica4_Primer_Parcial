#include "app_context.hpp"
#include "app_config.hpp"
#include "sensor_task.hpp"
#include "button_task.hpp"
#include "servo_task.hpp"
#include "ready_led_task.hpp"
#include "task_manager.hpp"

#include "esp_log.h"

static const char *TAG = "MAIN";

extern "C" void app_main(void) {
    app_context_init();

    static SensorTaskConfig_t sensor_cfg = {
        .name = "Task_Sensor",
        .period_ms = SENSOR_PERIOD_MS,
        .median_window = MEDIAN_WINDOW
    };

    static ServoTaskConfig_t servo_cfg = {
        .name = "Task_Servo",
        .pin = PIN_SERVO,
        .channel = SERVO_CHANNEL,
        .timer = SERVO_TIMER,
        .mode = SERVO_MODE
    };

    static ButtonTaskConfig_t start_btn_cfg = {
        .name = "Task_ButtonStart",
        .pin = PIN_BUTTON_START,
        .id = ButtonId::START,
        .report_release = false
    };

    static ButtonTaskConfig_t speed_btn_cfg = {
        .name = "Task_ButtonSpeed",
        .pin = PIN_BUTTON_SPEED,
        .id = ButtonId::SPEED,
        .report_release = true
    };

    static ReadyLedConfig_t ready_led_cfg = {
        .name = "Task_ReadyLed",
        .pin = PIN_READY_LED
    };

    static ManagerTaskConfig_t manager_cfg = {
        .name = "TaskManager"
    };

    ESP_LOGI(TAG, "=== Practica 4 SETR iniciada ===");

    xTaskCreate(SensorTask, sensor_cfg.name, 4096, &sensor_cfg, 3, &g_handles.h_sensor);
    xTaskCreate(ServoTask, servo_cfg.name, 4096, &servo_cfg, 3, &g_handles.h_servo);
    xTaskCreate(ButtonTask, start_btn_cfg.name, 2048, &start_btn_cfg, 4, &g_handles.h_button_start);
    xTaskCreate(ButtonTask, speed_btn_cfg.name, 2048, &speed_btn_cfg, 4, &g_handles.h_button_speed);
    xTaskCreate(ReadyLedTask, ready_led_cfg.name, 2048, &ready_led_cfg, 1, &g_handles.h_ready_led);
    xTaskCreate(TaskManager, manager_cfg.name, 4096, &manager_cfg, 5, &g_handles.h_manager);
}