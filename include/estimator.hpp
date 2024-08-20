

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

    EstimatorBase() :
    t_integrated(0),
    pos{0},
    vel{0}
    {
        rot.eye();
        t_start = millis()/1000.0f;
    };
    virtual void init() {};
    virtual void update() {};

};

template <typename T>
class IMU_Estimator : public EstimatorBase {
public:
    T imu;
    
    IMU_Estimator() : EstimatorBase() {};

    void init() override;
    void update() override;
};

template class IMU_Estimator<BMI323>;