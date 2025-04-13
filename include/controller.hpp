
#pragma once

// #include <Arduino.h>
// #include "Fastor/Fastor.h"
// #include "estimator.hpp"
// #include "thrustControl.hpp"
// #include "pid.hpp"

// using namespace Fastor;

// using TF3 = Tensor<float,3>;
// using TF3x3 = Tensor<float,3,3>;


// class CascadedPID {
//     Rotors &_rot;
//     EstimatorBase &_est;
//     PID<TF3> _pos_pid;
//     PID<TF3> _vel_pid;

// public:
//     CascadedPID(Rotors &rot, EstimatorBase &est) : _rot(rot), _est(est) {};

//     void init();
//     void update();
// };