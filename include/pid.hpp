
#pragma once

#include <Arduino.h>
#include "Fastor/Fastor.h"

using namespace Fastor;

using TF3 = Tensor<float,3>;
using TF4 = Tensor<float,4>;
using TF6 = Tensor<float,6>;

template <class T>
class PID {
private:
    T _Kp = {0.0};
    T _Ki = {0.0};
    T _Kd = {0.0};
    T _integral = {0.0};
    T _last_state = {0.0};
    T _control_effort = {0.0};
    T _setpoint = {0.0};
    unsigned long _last_update = 0;
    float _get_dt();

public:
    void init(T &Kp, T &Ki, T &Kd);
    void set(T &setpoint);
    T& update(T &state);
    
};
