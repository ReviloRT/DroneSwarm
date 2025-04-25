#include "rotors.hpp"


void Rotors::init() {
    pinMode(PIN_ESC_EN,OUTPUT);
    digitalWrite(PIN_ESC_EN,LOW);

    pinMode(PIN_M1_EN,OUTPUT);
    pinMode(PIN_M2_EN,OUTPUT);
    pinMode(PIN_M3_EN,OUTPUT);
    pinMode(PIN_M4_EN,OUTPUT);

    digitalWrite(PIN_M1_EN,LOW);
    digitalWrite(PIN_M2_EN,LOW);
    digitalWrite(PIN_M3_EN,LOW);
    digitalWrite(PIN_M4_EN,LOW);

    pinMode(PIN_M1_PH,OUTPUT);
    pinMode(PIN_M2_PH,OUTPUT);
    pinMode(PIN_M3_PH,OUTPUT);
    pinMode(PIN_M4_PH,OUTPUT);

    digitalWrite(PIN_M1_PH,LOW);
    digitalWrite(PIN_M2_PH,LOW);
    digitalWrite(PIN_M3_PH,LOW);
    digitalWrite(PIN_M4_PH,LOW);

    analogWriteFrequency(10000);
    analogWriteResolution(8);
}

void Rotors::on(int duty) {
    analogWrite(PIN_ESC_EN,duty);
}

void Rotors::off() {
    analogWrite(PIN_ESC_EN,0);
}

void Rotors::power(MotorSelect motor, int power) {

    if (power > 0) {
        digitalWrite(motor_PH_pin(motor),LOW);
    } else {
        digitalWrite(motor_PH_pin(motor),HIGH);
    }

    analogWrite(motor_EN_pin(motor),power);
}

void Rotors::stop(MotorSelect motor) {
    analogWrite(motor_EN_pin(motor),0);
}

uint8_t Rotors::motor_EN_pin(MotorSelect motor){
    switch (motor) {
        case MOTOR_1:
            return PIN_M1_EN;
        case MOTOR_2:
            return PIN_M2_EN;
        case MOTOR_3:
            return PIN_M3_EN;
        case MOTOR_4:
            return PIN_M4_EN;
        default:
            return PIN_M1_EN;
    }
}
uint8_t Rotors::motor_PH_pin(MotorSelect motor){
    switch (motor) {
        case MOTOR_1:
            return PIN_M1_PH;
        case MOTOR_2:
            return PIN_M2_PH;
        case MOTOR_3:
            return PIN_M3_PH;
        case MOTOR_4:
            return PIN_M4_PH;
        default:
            return PIN_M1_PH;
    }
}