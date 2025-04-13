#include "thrustControl.hpp"

void ThrustControl::init() {
    rotors.init();
}

void ThrustControl::on() {
    rotors.on(ROTOR_PWM_RESOLUTION);
}

void ThrustControl::off() {
    rotors.stop(MOTOR_1);
    rotors.stop(MOTOR_2);
    rotors.stop(MOTOR_3);
    rotors.stop(MOTOR_4);
    rotors.off();
}

void ThrustControl::update(TF4 new_effort) {
    control_effort = new_effort;
    TF4 thrusts = mixer % control_effort;

    rotors.power(MOTOR_1,_thrust_to_pwm(thrusts(0)));
    rotors.power(MOTOR_2,_thrust_to_pwm(thrusts(1)));
    rotors.power(MOTOR_3,_thrust_to_pwm(thrusts(2)));
    rotors.power(MOTOR_4,_thrust_to_pwm(thrusts(3)));
}

int ThrustControl::_thrust_to_pwm(float thrust) {
    return static_cast<int>(min(max(thrust*THRUST_TO_PWM_SCALING,-1.0f),1.0f) * ROTOR_PWM_RESOLUTION);
}