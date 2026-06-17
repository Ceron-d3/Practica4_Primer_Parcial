#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <stdint.h>

enum class ButtonId {
    START,
    SPEED
};

enum class ButtonAction {
    PRESSED,
    RELEASED
};

struct ButtonMsg_t {
    ButtonId id;
    ButtonAction action;
};

struct SensorMsg_t {
    int raw;
    int filtered;
    int target_angle;
};

struct ServoCmd_t {
    int target_angle;
    bool fast_mode;
};

struct ServoStatusMsg_t {
    int current_angle;
    int target_angle;
    bool arrived;
};

#endif