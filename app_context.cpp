#include "app_context.hpp"
#include "messages.hpp"

AppQueues_t g_queues;
AppHandles_t g_handles;

void app_context_init(void) {
    g_queues.q_button = xQueueCreate(10, sizeof(ButtonMsg_t));
    g_queues.q_sensor = xQueueCreate(10, sizeof(SensorMsg_t));
    g_queues.q_servo_cmd = xQueueCreate(10, sizeof(ServoCmd_t));
    g_queues.q_servo_status = xQueueCreate(10, sizeof(ServoStatusMsg_t));

    g_handles.h_sensor = nullptr;
    g_handles.h_servo = nullptr;
    g_handles.h_button_start = nullptr;
    g_handles.h_button_speed = nullptr;
    g_handles.h_ready_led = nullptr;
    g_handles.h_manager = nullptr;
}