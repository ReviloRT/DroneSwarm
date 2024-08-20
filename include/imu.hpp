
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
    uint32_t _prev_sample_t;
    TensorMap<float,3> _sample_accel;
    TensorMap<float,3> _sample_omega;

    float _Delta_t;
    TF3 _Delta_p;
    TF3 _Delta_v;
    TF3x3 _Delta_R;
    
    auto _rot_exp(TMF3 &vect,float dt);
    void _preintegrate();

protected:
    uint32_t _this_sample_t;
    float _accels[3];
    float _omegas[3];
    virtual void _read() {};
    
public:
    Preintegrator() : 
        _prev_sample_t(0), 
        _this_sample_t(0), 
        _accels{0},
        _omegas{0},
        _sample_accel(_accels), 
        _sample_omega(_omegas),
        _Delta_t(0),
        _Delta_p(0),
        _Delta_v(0) 
        { _Delta_R.eye();};
    void update();
    float get_t();
    TF3 get_p();
    TF3 get_v();
    TF3x3 get_R();
    void zero_integral();
    void print();

};

class BMI323: public Preintegrator {
protected:
    void _writeRegister16(uint16_t reg, uint16_t value);
    uint16_t _readRegister16(uint8_t reg);
    void _read() override ;
    float _temp;
public:
    BMI323() : Preintegrator(), _temp(0){};
    void init();
    void print_raw();
};

void print_TF3(TF3 &v);
void print_TF3x3(TF3x3 &m);