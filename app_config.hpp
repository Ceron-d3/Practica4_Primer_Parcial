#ifndef APP_CONFIG_HPP
#define APP_CONFIG_HPP

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"

constexpr gpio_num_t PIN_READY_LED = GPIO_NUM_2;
constexpr gpio_num_t PIN_BUTTON_START = GPIO_NUM_22;
constexpr gpio_num_t PIN_BUTTON_SPEED = GPIO_NUM_21;

constexpr gpio_num_t PIN_SERVO = GPIO_NUM_25;

constexpr adc_channel_t LDR_ADC_CHANNEL = ADC_CHANNEL_6; // GPIO34
constexpr adc_unit_t LDR_ADC_UNIT = ADC_UNIT_1;

constexpr ledc_timer_t SERVO_TIMER = LEDC_TIMER_0;
constexpr ledc_channel_t SERVO_CHANNEL = LEDC_CHANNEL_0;
constexpr ledc_mode_t SERVO_MODE = LEDC_LOW_SPEED_MODE;

constexpr int SERVO_FREQ_HZ = 50;
constexpr int SERVO_MIN_US = 500;
constexpr int SERVO_MAX_US = 2500;
constexpr int SERVO_MIN_ANGLE = 0;
constexpr int SERVO_MAX_ANGLE = 180;

constexpr int LIGHT_THRESHOLD = 2000;

constexpr int SENSOR_PERIOD_MS = 400;
constexpr int MEDIAN_WINDOW = 5;

constexpr int READY_LED_ON_MS = 250;
constexpr int READY_LED_OFF_MS = 100;

constexpr int BUTTON_POLL_MS = 20;
constexpr int BUTTON_DEBOUNCE_MS = 200;

constexpr int SERVO_SLOW_STEP_MS = 60;
constexpr int SERVO_FAST_STEP_MS = 30;

constexpr int HOLD_TARGET_MS = 8000;

#endif