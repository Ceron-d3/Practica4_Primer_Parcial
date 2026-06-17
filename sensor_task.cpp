#include "sensor_task.hpp"
#include "app_config.hpp"
#include "app_context.hpp"
#include "messages.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

static const char *TAG = "SENSOR";

static adc_oneshot_unit_handle_t adc_handle;

static void ordenar(int *data, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (data[j] < data[i]) {
                int temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}

static int obtener_mediana(int *data, int n) {
    ordenar(data, n);
    return data[n / 2];
}

static int angle_from_light(int value) {
    if (value >= LIGHT_THRESHOLD) {
        return 180;
    }

    return 0;
}

void SensorTask(void *pvParameters) {
    const SensorTaskConfig_t *cfg = static_cast<const SensorTaskConfig_t *>(pvParameters);

    adc_oneshot_unit_init_cfg_t init_cfg = {};
    init_cfg.unit_id = LDR_ADC_UNIT;
    init_cfg.ulp_mode = ADC_ULP_MODE_DISABLE;
    adc_oneshot_new_unit(&init_cfg, &adc_handle);

    adc_oneshot_chan_cfg_t chan_cfg = {};
    chan_cfg.atten = ADC_ATTEN_DB_12;
    chan_cfg.bitwidth = ADC_BITWIDTH_12;
    adc_oneshot_config_channel(adc_handle, LDR_ADC_CHANNEL, &chan_cfg);

    ESP_LOGI(TAG, "[%s] tarea iniciada", cfg->name);

    while (true) {
        int muestras[15] = {};
        int ventana = cfg->median_window;

        if (ventana > 15) {
            ventana = 15;
        }

        if (ventana < 3) {
            ventana = 3;
        }

        for (int i = 0; i < ventana; i++) {
            adc_oneshot_read(adc_handle, LDR_ADC_CHANNEL, &muestras[i]);
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        int filtered = obtener_mediana(muestras, ventana);
        int target = angle_from_light(filtered);

        SensorMsg_t msg = {};
        msg.raw = muestras[ventana / 2];
        msg.filtered = filtered;
        msg.target_angle = target;

        xQueueSend(g_queues.q_sensor, &msg, 0);

        ESP_LOGI(TAG, "raw=%d filtered=%d target=%d", msg.raw, msg.filtered, msg.target_angle);

        vTaskDelay(pdMS_TO_TICKS(cfg->period_ms));
    }
}