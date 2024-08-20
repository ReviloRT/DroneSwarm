#include <Arduino.h>
#include <Wire.h>
#include "imu.hpp"

#include "led.hpp"
#include "rotors.hpp"

Rotors rotors;


#define PIN_LED GPIO_NUM_2

BMI323 imu;

void blink(uint8_t pin, uint32_t millis);

void setup() {
  init_leds();

  delay(1000);
  rotors.on(128);
  Serial.begin(115200);

  digitalWrite(PIN_LED,HIGH);

  imu.init();
}

void loop() {
  imu.update();
}

void blink(uint8_t pin, uint32_t millis) {
  digitalWrite(pin,HIGH);
  delay(millis);
  digitalWrite(pin,LOW);
  delay(millis);
}
