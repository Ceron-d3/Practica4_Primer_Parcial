#ifndef SERVO_TASK_HPP
#define SERVO_TASK_HPP

#include "driver/gpio.h"
#include "driver/ledc.h"

struct ServoTaskConfig_t {
    const char *name;
    gpio_num_t pin;
    ledc_channel_t channel;
    ledc_timer_t timer;
    ledc_mode_t mode;
};

void ServoTask(void *pvParameters);

#endif