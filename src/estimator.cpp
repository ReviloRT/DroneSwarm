
#include "estimator.hpp"

void EstimatorBase::print() {
    Serial.printf(">t:%f\n", t_integrated);
    Serial.printf(">px:%f\n",pos(0));
    Serial.printf(">py:%f\n",pos(1));
    Serial.printf(">pz:%f\n",pos(2));
    Serial.printf(">vx:%f\n",vel(0));
    Serial.printf(">vy:%f\n",vel(1));
    Serial.printf(">vz:%f\n",vel(2));
    Serial.printf("R:[\n[%f, %f, %f]\n[%f, %f, %f]\n[%f, %f, %f]\n] \n", rot(0,0), rot(0,1), rot(0,2), rot(1,0), rot(1,1), rot(1,2), rot(2,0), rot(2,1), rot(2,2));
}

template <typename T>
void IMU_Estimator<T>::init() {
    imu.init();
    // TODO: Schedule imu to continue integrating
}

template <typename T>
void IMU_Estimator<T>::update() {
    if (t_start == 0) { t_start = millis()/1000.0f; return;}
    float t_now = (millis()/1000.0f) - t_start;
    t_integrated += imu.Delta_t;
    // if (abs(t_now - t_integrated) > 0.01) {
    //     Serial.printf("Integrator time is out of sync by %f seconds!\n",abs(t_now - t_integrated));
    // }

    rot = evaluate(rot % imu.Delta_R);
    pos += imu.Delta_t*vel + 0.5*imu.Delta_t*imu.Delta_t*gravity + evaluate(rot % imu.Delta_p);
    vel += imu.Delta_t*gravity + evaluate(rot % imu.Delta_v);

    Serial.printf(">gax:%f\n",gravity(0));
    Serial.printf(">gay:%f\n",gravity(1));
    Serial.printf(">gaz:%f\n",gravity(2));

    imu.zero_integral();
    update_counter ++;
}

template <typename T>
void IMU_Estimator<T>::set_gravity() {
    TF3 accelerations;
    int counter = 0;
    int tstart = millis();
    while (millis() < tstart + 1000*5) {
        counter++;
        imu.update();
        accelerations += imu.get_accelerations();
    }
    imu.zero_integral();
    gravity = -accelerations/counter;

}
