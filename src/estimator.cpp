
#include "estimator.hpp"

template <typename T>
void IMU_Estimator<T>::init() {
    imu.init();
    // TODO: Schedule imu to continue integrating
}

template <typename T>
void IMU_Estimator<T>::update() {
    float t_now = (millis()/1000.0f) - t_start;
    t_integrated += imu.Delta_t;
    if (abs(t_now - t_integrated) > 0.01) {
        Serial.printf("Integrator time is out of sync by %f seconds!\n",abs(t_now - t_integrated));
    }

    pos += imu.Delta_p;
    vel += imu.Delta_v;
    rot += imu.Delta_R;
    vel(2) += 9.81 * imu.Delta_t;

    imu.zero_integral();
}