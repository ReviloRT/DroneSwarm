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
  set_RGB_LED(0,1,0);
  
  uint32_t end1 = millis() + 100;
  uint64_t waited = 0;
  int counter = 0;
  while (millis() < end1) {
    uint64_t end2 = micros() + 1000;
    imu.update();
    waited += max(end2 - micros(),0ull);
    while (micros() < end2) {
      asm("NOP");
    }
    counter ++;
  }
  // imu.print_raw();
  // imu.print();

  Serial.printf("Waited: %f micros \n",(double)waited/(double)counter);

  TF3 up_unit = {0,0,1};
  TF3 up = imu.Delta_R % up_unit;
  Serial.printf(">ux:%f\n",up(0));
  Serial.printf(">uy:%f\n",up(1));
  Serial.printf(">uz:%f\n",up(2)); 

  imu.zero_integral();
}