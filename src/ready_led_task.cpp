#include "ready_led_task.hpp"
#include "app_config.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "READY_LED";
static gpio_num_t ready_pin = PIN_READY_LED;

void ready_led_force_off(void) {
    gpio_set_level(ready_pin, 0);
}

void ReadyLedTask(void *pvParameters) {
    const ReadyLedConfig_t *cfg = static_cast<const ReadyLedConfig_t *>(pvParameters);
    ready_pin = cfg->pin;

    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << cfg->pin);
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);

    ESP_LOGI(TAG, "[%s] tarea iniciada", cfg->name);

    while (true) {
        gpio_set_level(cfg->pin, 1);
        vTaskDelay(pdMS_TO_TICKS(READY_LED_ON_MS));

        gpio_set_level(cfg->pin, 0);
        vTaskDelay(pdMS_TO_TICKS(READY_LED_OFF_MS));
    }
}
