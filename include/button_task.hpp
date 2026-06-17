#ifndef BUTTON_TASK_HPP
#define BUTTON_TASK_HPP

#include "driver/gpio.h"
#include "messages.hpp"

struct ButtonTaskConfig_t {
    const char *name;
    gpio_num_t pin;
    ButtonId id;
    bool report_release;
};

void ButtonTask(void *pvParameters);

#endif
