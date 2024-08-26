#include "pid.hpp"

template <class T>
float PID<T>::_get_dt() {
    unsigned long now = micros();
    float dt = (now - _last_update)/1000000.0f;
    _last_update = now;
    return dt;
}

template <class T>
void PID<T>::init(T &Kp, T &Ki, T &Kd) {
    _Kp = Kp;
    _Ki = Ki;
    _Kd = Kd;
}

template <class T>
T& PID<T>::update(T &state) {
    float dt = _get_dt();
    auto error = _setpoint - state;

    auto deriv = (state - _last_state) * dt;
    _control_effort = _Kp * error + _Ki * _integral + _Kd * deriv;

    _integral += error * dt;
    _last_state = state;
    return _control_effort;
}