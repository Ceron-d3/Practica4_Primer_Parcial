#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

struct ManagerTaskConfig_t {
    const char *name;
};

void TaskManager(void *pvParameters);

#endif