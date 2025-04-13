#include <Arduino.h>
#include <Wire.h>
#include "estimator.hpp"
#include "led.hpp"
#include "thrustControl.hpp"

ThrustControl thrust;
IMU_Estimator<BMI323> est;

class Timer {
  int next_time = 0;
  float rate = 1;

  public:

  Timer(int rate): rate(rate) {};
  
  bool run() {
    if (next_time == 0) {
      next_time = millis() + 1.0/rate;
      return false;
    }
    int now = millis();
    if (next_time < now) {
      next_time += 1.0/rate; 
      return true;
    }
    return false;
  }
};

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

Timer timerA(10);
Timer timerB(5);

void loop() {
  est.imu.update();

  if (timerA.run()) {
    est.update();
    Serial.print("Integrations: ");
    Serial.println(est.update_counter);
    Serial.print("Reads: ");
    Serial.println(est.imu.read_counter);
    Serial.print("Preintegrations: ");
    Serial.println(est.imu.integrate_counter);
    est.print();
    est.imu.print();
    est.imu.print_raw();
  }

  if (timerB.run()) {
    TF4 effort = {0.1,0.0,0.0,0.0};
    thrust.update(effort);
  }
  

}