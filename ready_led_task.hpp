#ifndef READY_LED_TASK_HPP
#define READY_LED_TASK_HPP

#include "driver/gpio.h"

struct ReadyLedConfig_t {
    const char *name;
    gpio_num_t pin;
};

void ReadyLedTask(void *pvParameters);
void ready_led_force_off(void);

#endif