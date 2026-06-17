/**
 * Respuestas:
 *
 * 1. Una variable global comparte datos directamente, pero no sincroniza tareas.
 *    Una cola permite enviar datos entre tareas de forma ordenada y segura.
 *
 * 2. La tarea que ejecuta xQueueReceive() queda bloqueada si espera datos
 *    y la cola está vacía.
 *
 * 3. TaskManager concentra las decisiones para evitar que Sensor, Servo
 *    o Botones modifiquen directamente el estado general del sistema.
 *
 * 4. pvParameters permite usar la misma funcion de tarea con diferentes
 *    configuraciones, evitando crear funciones repetidas.
 *
 * 5. Una tarea suspendida no vuelve a ejecutarse hasta recibir vTaskResume().
 *    Una tarea bloqueada por una cola vuelve cuando recibe datos o vence
 *    el tiempo de espera.
 *
 * 6. Aumentar la ventana del filtro de mediana mejora el rechazo de ruido,
 *    pero hace que la respuesta sea mas lenta.
 *
 * 7. La mediana rechaza picos mejor que el promedio porque toma el valor
 *    central ordenado y no se ve tan afectada por valores extremos.
 *
 * 8. Si se presiona Start-operation durante el movimiento, no debe reiniciar
 *    la operacion porque la tarea del boton Start queda suspendida.
 *
 * 9. El boton de velocidad solo funciona durante la operacion porque
 *    TaskManager lo reanuda al iniciar y lo suspende al finalizar.
 *
 * 10. constexpr es preferible a #define porque tiene tipo, respeta alcance
 *     y permite verificacion por el compilador.
 *CONCLUSION

En esta práctica se implementó un sistema multitarea utilizando FreeRTOS y ESP-IDF para controlar un servomotor mediante eventos, colas y un filtro de mediana. El uso de TaskManager permitió centralizar la lógica de control, mientras que las colas facilitaron una comunicación segura entre tareas. Además, se comprobó el funcionamiento de la suspensión y reanudación de tareas mediante TaskHandle_t, obteniendo un sistema modular, escalable y fácil de mantener.
*/
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
