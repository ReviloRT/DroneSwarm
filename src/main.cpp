#include <Arduino.h>
#include <Wire.h>
#include "imu.hpp"
#include "led.hpp"
#include "controller.hpp"

Controller control;
BMI323 imu;

void setup() {
  init_leds();
  Serial.begin(115200);

  digitalWrite(PIN_LED,HIGH);
  set_RGB_LED(1,1,1);

  imu.init();
  control.init();

  delay(1000);
  control.on();
}

void loop() {

  control.on();

  set_RGB_LED(1,1,0);
  for (size_t i = 0; i < 100; i++) {
    control.update({i/100.0f,0.0f,0.0f,0.0f});
    delay(20);
  }

  set_RGB_LED(0,1,0);
  delay(2000);

  set_RGB_LED(0,1,1);
  control.update({0.0f,1.0f,0.0f,0.0f});
  delay(1000);

  set_RGB_LED(0,0,1);
  control.update({0.0f,0.0f,1.0f,0.0f});
  delay(1000);

  set_RGB_LED(1,0,1);
  control.update({0.0f,0.0f,0.0f,1.0f});
  delay(1000);

  set_RGB_LED(1,1,0);
  for (size_t i = 0; i < 100; i++) {
    control.update({1.0f-i/100.0f,0.0f,0.0f,0.0f});
    delay(20);
  }
  
}