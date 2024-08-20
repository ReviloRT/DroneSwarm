#include <Arduino.h>
#include <Wire.h>
#include "imu.hpp"
#include "led.hpp"

BMI323 imu;

void setup() {
  init_leds();
  Serial.begin(115200);

  digitalWrite(PIN_LED,HIGH);
  set_RGB_LED(1,1,1);

  imu.init();
}

void loop() {
  imu.update();
  set_RGB_LED(0,1,0);
}