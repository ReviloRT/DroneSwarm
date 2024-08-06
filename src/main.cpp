#include <Arduino.h>

#define PIN_LED GPIO_NUM_2

void blink(uint8_t pin, uint32_t millis);

void setup() {
  pinMode(PIN_LED,OUTPUT);
}

void loop() {
  blink(PIN_LED,250);
}

void blink(uint8_t pin, uint32_t millis) {
  digitalWrite(pin,HIGH);
  delay(millis);
  digitalWrite(pin,LOW);
  delay(millis);
}