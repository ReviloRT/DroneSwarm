
#include "controller.hpp"

// // Cascaded CascadedPID steps
// // 1. compare current to desired position -> generate desired velocity vector towards target
// // 2. compare current to desired velocity -> generate desired acceleration vector -> compensate gravity vector
// // 3. convert acceleration vector to thrust, roll, pitch, yaw thrusts
// //      set thrust = vector mag
// //      set roll, pitch, yaw = rates towards new angle = ????


// void CascadedPID::init() {
//     // _rot.init();
//     // _est.init();
//     // _pos_pid.init(1,0,0);
//     // _vel_pid.init(1,0,0);
// }

// void Controller::update(TF4 new_effort) {
//     control_effort(all) = new_effort(all);
//     TF4 thrusts = mixer % control_effort;

//     rotors.power(MOTOR_1,_thrust_to_pwm(thrusts(0)));
//     rotors.power(MOTOR_2,_thrust_to_pwm(thrusts(1)));
//     rotors.power(MOTOR_3,_thrust_to_pwm(thrusts(2)));
//     rotors.power(MOTOR_4,_thrust_to_pwm(thrusts(3)));
// }

// int Controller::_thrust_to_pwm(float thrust) {
//     return static_cast<int>(min(max(thrust*THRUST_TO_PWM_SCALING,-1.0f),1.0f) * ROTOR_PWM_RESOLUTION);
// }
