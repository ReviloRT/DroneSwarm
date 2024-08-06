#include "led.hpp"


void blink(uint8_t pin, uint32_t millis) {
  digitalWrite(pin,HIGH);
  delay(millis);
  digitalWrite(pin,LOW);
  delay(millis);
}

void init_leds() {
  pinMode(PIN_LED,OUTPUT);
  pinMode(PIN_RGB_R,OUTPUT);
  pinMode(PIN_RGB_G,OUTPUT);
  pinMode(PIN_RGB_B,OUTPUT);
  digitalWrite(PIN_LED,HIGH);
}

void set_RGB_LED(bool R_on, bool G_on, bool B_on) {
  digitalWrite(PIN_RGB_R,R_on);
  digitalWrite(PIN_RGB_G,G_on);
  digitalWrite(PIN_RGB_B,B_on);
}