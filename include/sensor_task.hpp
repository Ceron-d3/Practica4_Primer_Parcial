#ifndef SENSOR_TASK_HPP
#define SENSOR_TASK_HPP

struct SensorTaskConfig_t {
    const char *name;
    int period_ms;
    int median_window;
};

void SensorTask(void *pvParameters);

#endif
