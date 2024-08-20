#pragma once

#include <Arduino.h>
#include "rotors.hpp"
#include "Fastor/Fastor.h"

#define QUAD_MIXER_LENGTH 1
#define QUAD_MIXER_GAMMA 1
#define ROTOR_PWM_RESOLUTION 128
#define THRUST_TO_PWM_SCALING 5.0f

using namespace Fastor;

using TF4 = Tensor<float,4>;
using TF4x4 = Tensor<float,4,4>;

class ThrustControl {
private:
    Rotors rotors;
    const TF4x4 mixer;
    
    int _thrust_to_pwm(float thrust);

public:
    TF4 control_effort;

    ThrustControl() : 
        control_effort{0},
        mixer{
            { 0.25,-0.25/QUAD_MIXER_LENGTH, 0.25/QUAD_MIXER_LENGTH, 0.25/QUAD_MIXER_GAMMA},
            { 0.25, 0.25/QUAD_MIXER_LENGTH,-0.25/QUAD_MIXER_LENGTH, 0.25/QUAD_MIXER_GAMMA},
            { 0.25, 0.25/QUAD_MIXER_LENGTH, 0.25/QUAD_MIXER_LENGTH,-0.25/QUAD_MIXER_GAMMA},
            { 0.25,-0.25/QUAD_MIXER_LENGTH,-0.25/QUAD_MIXER_LENGTH,-0.25/QUAD_MIXER_GAMMA}
        } {};

    void init();
    void on();
    void off();
    void update(TF4 control_effort);
    
};