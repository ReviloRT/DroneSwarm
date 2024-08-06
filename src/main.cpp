#include <Arduino.h>

#define PIN_LED GPIO_NUM_2
#define PIN_RGB_R GPIO_NUM_8
#define PIN_RGB_G GPIO_NUM_9
#define PIN_RGB_B GPIO_NUM_10

void blink(uint8_t pin, uint32_t millis);
void set_RGB_LED(bool R_on, bool G_on, bool B_on);

void setup() {
  pinMode(PIN_LED,OUTPUT);
  pinMode(PIN_RGB_R,OUTPUT);
  pinMode(PIN_RGB_G,OUTPUT);
  pinMode(PIN_RGB_B,OUTPUT);
  set_RGB_LED(1,1,1);
  delay(1000);
}

void loop() {
  blink(PIN_LED,100);
  
  set_RGB_LED(1,0,0);
  delay(200);
  set_RGB_LED(0,1,0);
  delay(200);
  set_RGB_LED(0,0,1);
  delay(200);
}

void blink(uint8_t pin, uint32_t millis) {
  digitalWrite(pin,HIGH);
  delay(millis);
  digitalWrite(pin,LOW);
  delay(millis);
}

void set_RGB_LED(bool R_on, bool G_on, bool B_on) {
  digitalWrite(PIN_RGB_R,R_on);
  digitalWrite(PIN_RGB_G,G_on);
  digitalWrite(PIN_RGB_B,B_on);
}