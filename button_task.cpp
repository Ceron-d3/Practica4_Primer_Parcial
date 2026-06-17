#include "button_task.hpp"
#include "app_context.hpp"
#include "app_config.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "BUTTON";

void ButtonTask(void *pvParameters) {
    const ButtonTaskConfig_t *cfg = static_cast<const ButtonTaskConfig_t *>(pvParameters);

    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << cfg->pin);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);

    int last_state = 1;

    ESP_LOGI(TAG, "[%s] tarea iniciada en GPIO%d", cfg->name, cfg->pin);

    while (true) {
        int current_state = gpio_get_level(cfg->pin);

        if (last_state == 1 && current_state == 0) {
            ButtonMsg_t msg = {cfg->id, ButtonAction::PRESSED};
            xQueueSend(g_queues.q_button, &msg, 0);
            ESP_LOGI(TAG, "[%s] PRESSED", cfg->name);
            vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE_MS));
        }

        if (cfg->report_release && last_state == 0 && current_state == 1) {
            ButtonMsg_t msg = {cfg->id, ButtonAction::RELEASED};
            xQueueSend(g_queues.q_button, &msg, 0);
            ESP_LOGI(TAG, "[%s] RELEASED", cfg->name);
            vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE_MS));
        }

        last_state = current_state;
        vTaskDelay(pdMS_TO_TICKS(BUTTON_POLL_MS));
    }
}