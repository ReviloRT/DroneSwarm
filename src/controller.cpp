
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

// void CascadedPID::update() {
//     // TF3 vel_t = _pos_pid.update(_est.pos);
//     // TF3 acc_t = _vel_pid.update(_est.vel - vel_t);
    
//     // TF4 control_effort(0);
//     // control_effort[0] = acc_t.mag();
//     // control
    

// }