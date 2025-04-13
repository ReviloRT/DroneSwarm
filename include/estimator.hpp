

#pragma once

#include <Arduino.h>
#include "Fastor/Fastor.h"
#include "imu.hpp"

using namespace Fastor;

using TF3 = Tensor<float,3>;
using TF3x3 = Tensor<float,3,3>;

class EstimatorBase {
protected:
    float t_start;
    float t_integrated;

public:
    TF3 pos;
    TF3 vel;
    TF3x3 rot;
    uint32_t update_counter = 0;

    EstimatorBase() :
    t_integrated(0.0f),
    pos{0.0f,0.0f,0.0f},
    vel{0.0f,0.0f,0.0f}
    {
        rot.eye();
        t_start = 0;
    };
    virtual void init() {};
    virtual void update() {};
    void print();

};

template <typename T>
class IMU_Estimator : public EstimatorBase {
public:
    T imu;
    TF3 gravity{0.0f,0.0f,0.0f};
    
    IMU_Estimator() : EstimatorBase() {};

    void init() override;
    void update() override;
    void set_gravity();
};

template class IMU_Estimator<BMI323>;