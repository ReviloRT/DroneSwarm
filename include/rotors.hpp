#pragma once

#include <Arduino.h>

#define PIN_ESC_EN GPIO_NUM_21

#define PIN_M1_EN GPIO_NUM_15
#define PIN_M2_EN GPIO_NUM_16
#define PIN_M3_EN GPIO_NUM_17
#define PIN_M4_EN GPIO_NUM_18

#define PIN_M1_PH GPIO_NUM_11
#define PIN_M2_PH GPIO_NUM_12
#define PIN_M3_PH GPIO_NUM_13
#define PIN_M4_PH GPIO_NUM_14

enum MotorSelect {
    MOTOR_1,
    MOTOR_2,
    MOTOR_3,
    MOTOR_4
};

class Rotors {
private:
    float power_1 = 0;
    float power_2 = 0;
    float power_3 = 0;
    float power_4 = 0;
    uint8_t motor_EN_pin(MotorSelect motor);
    uint8_t motor_PH_pin(MotorSelect motor);

public:
    void init();
    void on(int duty);
    void off();
    void power(MotorSelect motor, int power);
    void stop(MotorSelect motor);
};