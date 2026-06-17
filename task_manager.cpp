#include "task_manager.hpp"
#include "app_context.hpp"
#include "messages.hpp"
#include "ready_led_task.hpp"
#include "app_config.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "MANAGER";

enum class SystemState {
    READY,
    OPERATING,
    HOLDING
};

void TaskManager(void *pvParameters) {
    const ManagerTaskConfig_t *cfg = static_cast<const ManagerTaskConfig_t *>(pvParameters);
    (void)cfg;

    SystemState state = SystemState::READY;

    bool fast_mode = false;
    int last_target = 0;

    TickType_t hold_start = 0;
    TickType_t last_log = 0;

    vTaskSuspend(g_handles.h_sensor);
    vTaskSuspend(g_handles.h_servo);
    vTaskSuspend(g_handles.h_button_speed);

    ESP_LOGI(TAG, "Sistema listo. Presiona START");

    while (true) {
        ButtonMsg_t button_msg = {};
        SensorMsg_t sensor_msg = {};
        ServoStatusMsg_t servo_status = {};

        if (xQueueReceive(g_queues.q_button, &button_msg, pdMS_TO_TICKS(50)) == pdTRUE) {
            if (button_msg.id == ButtonId::START && button_msg.action == ButtonAction::PRESSED) {
                if (state == SystemState::READY) {
                    state = SystemState::OPERATING;
                    fast_mode = false;

                    ready_led_force_off();

                    vTaskSuspend(g_handles.h_ready_led);
                    vTaskSuspend(g_handles.h_button_start);

                    vTaskResume(g_handles.h_sensor);
                    vTaskResume(g_handles.h_servo);
                    vTaskResume(g_handles.h_button_speed);

                    ESP_LOGI(TAG, "Operacion iniciada");
                } else {
                    ESP_LOGW(TAG, "START ignorado: operacion en curso");
                }
            }

            if (button_msg.id == ButtonId::SPEED && state == SystemState::OPERATING) {
                fast_mode = (button_msg.action == ButtonAction::PRESSED);

                ServoCmd_t cmd = {};
                cmd.target_angle = last_target;
                cmd.fast_mode = fast_mode;

                xQueueSend(g_queues.q_servo_cmd, &cmd, 0);

                ESP_LOGI(TAG, "Velocidad=%s", fast_mode ? "RAPIDA" : "LENTA");
            }
        }

        if (state == SystemState::OPERATING) {
            if (xQueueReceive(g_queues.q_sensor, &sensor_msg, 0) == pdTRUE) {
                last_target = sensor_msg.target_angle;

                ServoCmd_t cmd = {};
                cmd.target_angle = last_target;
                cmd.fast_mode = fast_mode;

                xQueueSend(g_queues.q_servo_cmd, &cmd, 0);

                ESP_LOGI(TAG,
                         "Sensor filtered=%d target=%d",
                         sensor_msg.filtered,
                         sensor_msg.target_angle);
            }

            if (xQueueReceive(g_queues.q_servo_status, &servo_status, 0) == pdTRUE) {
                if (servo_status.arrived) {
                    state = SystemState::HOLDING;
                    hold_start = xTaskGetTickCount();

                    ESP_LOGI(TAG, "Objetivo alcanzado. Manteniendo 8 segundos");
                }
            }
        }

        if (state == SystemState::HOLDING) {
            TickType_t elapsed = xTaskGetTickCount() - hold_start;

            if (elapsed >= pdMS_TO_TICKS(HOLD_TARGET_MS)) {
                state = SystemState::READY;
                fast_mode = false;

                vTaskSuspend(g_handles.h_sensor);
                vTaskSuspend(g_handles.h_servo);
                vTaskSuspend(g_handles.h_button_speed);

                vTaskResume(g_handles.h_button_start);
                vTaskResume(g_handles.h_ready_led);

                ESP_LOGI(TAG, "Operacion terminada. Sistema listo nuevamente");
            }
        }

        if ((xTaskGetTickCount() - last_log) >= pdMS_TO_TICKS(3000)) {
            last_log = xTaskGetTickCount();

            ESP_LOGI(TAG,
                     "Estado=%d Sensor=%d Servo=%d Start=%d Speed=%d",
                     static_cast<int>(state),
                     static_cast<int>(eTaskGetState(g_handles.h_sensor)),
                     static_cast<int>(eTaskGetState(g_handles.h_servo)),
                     static_cast<int>(eTaskGetState(g_handles.h_button_start)),
                     static_cast<int>(eTaskGetState(g_handles.h_button_speed)));
        }
    }
}
