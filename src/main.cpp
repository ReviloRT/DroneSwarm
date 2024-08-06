#include <Arduino.h>

#include "led.hpp"

void setup() {
  init_leds();
  delay(1000);
}

void loop() {
  set_RGB_LED(1,0,0);
  delay(100);
  set_RGB_LED(1,1,0);
  delay(100);
  set_RGB_LED(0,1,0);
  delay(100);
  set_RGB_LED(0,1,1);
  delay(100);
  set_RGB_LED(0,0,1);
  delay(100);
  set_RGB_LED(1,0,1);
  delay(100);
}
