#include <Arduino.h>

#include "led.hpp"
#include "rotors.hpp"

Rotors rotors;

void setup() {
  init_leds();
  rotors.init();

  delay(1000);
  rotors.on(128);
}


void loop() {

  set_RGB_LED(1,1,0);
  for (size_t i = 0; i < 255; i++) {
    rotors.power(MOTOR_1,i);
    rotors.power(MOTOR_2,i);
    rotors.power(MOTOR_3,i);
    rotors.power(MOTOR_4,i);
    delay(10);
  }
  set_RGB_LED(0,1,0);
  delay(1000);

  set_RGB_LED(1,1,0);
  for (size_t i = 0; i < 255; i++) {
    rotors.power(MOTOR_1,255-i);
    rotors.power(MOTOR_2,255-i);
    rotors.power(MOTOR_3,255-i);
    rotors.power(MOTOR_4,255-i);
    delay(10);
  }

  set_RGB_LED(1,0,0);
  rotors.stop(MOTOR_1);
  rotors.stop(MOTOR_2);
  rotors.stop(MOTOR_3);
  rotors.stop(MOTOR_4);
  delay(1000);

}
