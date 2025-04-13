#include <Arduino.h>
#include <Wire.h>
#include "estimator.hpp"
#include "led.hpp"
#include "thrustControl.hpp"

ThrustControl thrust;
IMU_Estimator<BMI323> est;

int counter = 0;

void setup() {
  init_leds();
  Serial.begin(115200);

  digitalWrite(PIN_LED,HIGH);
  set_RGB_LED(1,1,1);

  thrust.init();
  est.init();

  delay(1000);
  thrust.on();
  set_RGB_LED(0,1,1);

  est.set_gravity();

  set_RGB_LED(1,0,1);

}

void loop() {
  est.imu.update();

  if (counter*1000 < millis()*10) {
    counter ++;
    est.update();
    // if (counter % 10) {
      Serial.print("Integrations: ");
      Serial.println(est.update_counter);
      Serial.print("Reads: ");
      Serial.println(est.imu.read_counter);
      Serial.print("Preintegrations: ");
      Serial.println(est.imu.integrate_counter);
      est.print();
      est.imu.print();
      est.imu.print_raw();
    // }
  }
  
}