
#pragma once

#include <Arduino.h>
#include <Fastor/Fastor.h>
#include <Wire.h>

#define BMI323_ADR 0x68
#define PIN_SDA GPIO_NUM_35
#define PIN_SCL GPIO_NUM_36

using namespace Fastor;

using TF3 = Tensor<float,3>;
using TMF3 = TensorMap<float,3>;
using TF3x3 = Tensor<float,3,3>;

class Preintegrator {
private:
    TensorMap<float,3> _sample_accel;
    TensorMap<float,3> _sample_omega;
    
    auto _rot_exp(TMF3 &vect,float dt);
    void _preintegrate();

protected:
    uint32_t _prev_sample_t;
    uint32_t _this_sample_t;
    float _accels[3];
    float _omegas[3];
    virtual void _read() {};

    
public:
    float Delta_t;
    uint32_t read_counter = 0;
    uint32_t integrate_counter = 0;
    TF3 Delta_p;
    TF3 Delta_v;
    TF3x3 Delta_R;

    Preintegrator() : 
        _prev_sample_t(0.0f), 
        _this_sample_t(0.0f), 
        _accels{0.0f},
        _omegas{0.0f},
        _sample_accel(_accels), 
        _sample_omega(_omegas),
        Delta_t(0.0f),
        Delta_p(0.0f),
        Delta_v(0.0f) 
        { Delta_R.eye();};
    
    virtual void init() {};
    void update();
    void zero_integral();
    void print();
    TF3 get_accelerations();
};

class BMI323: public Preintegrator {
protected:
    void _writeRegister16(uint16_t reg, uint16_t value);
    uint16_t _readRegister16(uint8_t reg);
    void _read() override ;
    float _temp;
public:
    BMI323() : Preintegrator(), _temp(0.0f) {};
    void init() override;
    void print_raw();

};

void print_TF3(TF3 &v);
void print_TF3x3(TF3x3 &m);