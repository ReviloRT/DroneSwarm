
#pragma once

#include <Arduino.h>
#include <Fastor/Fastor.h>
#include <Wire.h>

#define BMI323_ADR 0x68
#define PIN_SDA GPIO_NUM_35
#define PIN_SCL GPIO_NUM_36

using namespace Fastor;

using TF3 = Tensor<float,3>;

class Preintegrator {
private:
    uint64_t _prev_sample_t;
    
    TensorMap<float,3> _sample_omega;
    TensorMap<float,3> _sample_accel;
    float _Delta_t;
    TF3 _Delta_p;
    TF3 _Delta_v;
    TF3 _Delta_R;
    
    void _preintegrate();

protected:
    uint64_t _this_sample_t;
    float _readings[6];
    virtual void _read() {};
    
public:
    Preintegrator() : 
        _prev_sample_t(0), 
        _this_sample_t(0), 
        _readings{0},
        _sample_omega(&_readings[0]), 
        _sample_accel(&_readings[3]), 
        _Delta_t(0),
        _Delta_p(0),
        _Delta_v(0),
        _Delta_R(0) {};
    void update();
    float get_t();
    TF3 get_p();
    TF3 get_v();
    TF3 get_R();
    void zero_integral();
    void print();

};

class BMI323: public Preintegrator {
private:
    void _writeRegister16(uint16_t reg, uint16_t value);
    uint16_t _readRegister16(uint8_t reg);
    void _read();
public:
    BMI323() : Preintegrator() {};
    void init();
};