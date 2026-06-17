#include "servo_task.hpp"
#include "app_config.hpp"
#include "app_context.hpp"
#include "messages.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "SERVO";

static int angle_to_duty(int angle) {
    if (angle < SERVO_MIN_ANGLE) {
        angle = SERVO_MIN_ANGLE;
    }

    if (angle > SERVO_MAX_ANGLE) {
        angle = SERVO_MAX_ANGLE;
    }

    int pulse_us = SERVO_MIN_US + ((SERVO_MAX_US - SERVO_MIN_US) * angle) / 180;
    int duty = (pulse_us * 65535) / 20000;

    return duty;
}

static void servo_write_angle(const ServoTaskConfig_t *cfg, int angle) {
    int duty = angle_to_duty(angle);

    ledc_set_duty(cfg->mode, cfg->channel, duty);
    ledc_update_duty(cfg->mode, cfg->channel);
}

void ServoTask(void *pvParameters) {
    const ServoTaskConfig_t *cfg = static_cast<const ServoTaskConfig_t *>(pvParameters);

    ledc_timer_config_t timer_cfg = {};
    timer_cfg.speed_mode = cfg->mode;
    timer_cfg.timer_num = cfg->timer;
    timer_cfg.duty_resolution = LEDC_TIMER_16_BIT;
    timer_cfg.freq_hz = SERVO_FREQ_HZ;
    timer_cfg.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&timer_cfg);

    ledc_channel_config_t channel_cfg = {};
    channel_cfg.gpio_num = cfg->pin;
    channel_cfg.speed_mode = cfg->mode;
    channel_cfg.channel = cfg->channel;
    channel_cfg.intr_type = LEDC_INTR_DISABLE;
    channel_cfg.timer_sel = cfg->timer;
    channel_cfg.duty = 0;
    channel_cfg.hpoint = 0;
    ledc_channel_config(&channel_cfg);

    int current_angle = 0;
    int target_angle = 0;
    bool fast_mode = false;

    servo_write_angle(cfg, current_angle);

    ESP_LOGI(TAG, "[%s] tarea iniciada", cfg->name);

    while (true) {
        ServoCmd_t cmd = {};

        if (xQueueReceive(g_queues.q_servo_cmd, &cmd, portMAX_DELAY) == pdTRUE) {
            target_angle = cmd.target_angle;
            fast_mode = cmd.fast_mode;

            ESP_LOGI(TAG, "nuevo objetivo=%d modo=%s",
                     target_angle,
                     fast_mode ? "RAPIDO" : "LENTO");

            while (current_angle != target_angle) {
                ServoCmd_t new_cmd = {};

                if (xQueueReceive(g_queues.q_servo_cmd, &new_cmd, 0) == pdTRUE) {
                    target_angle = new_cmd.target_angle;
                    fast_mode = new_cmd.fast_mode;

                    ESP_LOGI(TAG, "actualizacion objetivo=%d modo=%s",
                             target_angle,
                             fast_mode ? "RAPIDO" : "LENTO");
                }

                if (current_angle < target_angle) {
                    current_angle++;
                } else if (current_angle > target_angle) {
                    current_angle--;
                }

                servo_write_angle(cfg, current_angle);

                ServoStatusMsg_t status = {};
                status.current_angle = current_angle;
                status.target_angle = target_angle;
                status.arrived = false;
                xQueueSend(g_queues.q_servo_status, &status, 0);

                vTaskDelay(pdMS_TO_TICKS(fast_mode ? SERVO_FAST_STEP_MS : SERVO_SLOW_STEP_MS));
            }

            ServoStatusMsg_t status = {};
            status.current_angle = current_angle;
            status.target_angle = target_angle;
            status.arrived = true;
            xQueueSend(g_queues.q_servo_status, &status, 0);

            ESP_LOGI(TAG, "objetivo alcanzado=%d", target_angle);
        }
    }
}