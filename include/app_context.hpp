#ifndef APP_CONTEXT_HPP
#define APP_CONTEXT_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

struct AppQueues_t {
    QueueHandle_t q_button;
    QueueHandle_t q_sensor;
    QueueHandle_t q_servo_cmd;
    QueueHandle_t q_servo_status;
};

struct AppHandles_t {
    TaskHandle_t h_sensor;
    TaskHandle_t h_servo;
    TaskHandle_t h_button_start;
    TaskHandle_t h_button_speed;
    TaskHandle_t h_ready_led;
    TaskHandle_t h_manager;
};

extern AppQueues_t g_queues;
extern AppHandles_t g_handles;

void app_context_init(void);

#endif
