#include "controller.hpp"

void Controller::init() {
    rotors.init();
}

void Controller::on() {
    rotors.on(ROTOR_PWM_RESOLUTION);
}

void Controller::off() {
    rotors.stop(MOTOR_1);
    rotors.stop(MOTOR_2);
    rotors.stop(MOTOR_3);
    rotors.stop(MOTOR_4);
    rotors.off();
}

void Controller::update() {
    TF4 thrusts = mixer % control_effort;

    rotors.power(MOTOR_1,_thrust_to_pwm(thrusts(0)));
    rotors.power(MOTOR_2,_thrust_to_pwm(thrusts(1)));
    rotors.power(MOTOR_3,_thrust_to_pwm(thrusts(2)));
    rotors.power(MOTOR_4,_thrust_to_pwm(thrusts(3)));
}

int Controller::_thrust_to_pwm(float thrust) {
    return static_cast<int>(min(max(thrust*THRUST_TO_PWM_SCALING,-1.0f),1.0f) * 128);
}