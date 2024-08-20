#include <Arduino.h>
#include <Wire.h>
#include "estimator.hpp"
#include "led.hpp"
#include "thrustControl.hpp"

ThrustControl thrust;
IMU_Estimator<BMI323> est;

void setup() {
  init_leds();
  Serial.begin(115200);

  digitalWrite(PIN_LED,HIGH);
  set_RGB_LED(1,1,1);

  thrust.init();
  est.init();

  delay(1000);
  thrust.on();
}

void loop() {
  est.update();
  
}