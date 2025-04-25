#include <Arduino.h>
#include <Wire.h>
#include "estimator.hpp"
#include "led.hpp"
#include "thrustControl.hpp"

ThrustControl thrust;
IMU_Estimator<BMI323> est;

class Timer {
  int next_time = 0;
  float timer_millis = 1;

  public:

  Timer(int timer_millis): timer_millis(timer_millis) {};
  
  bool run() {
    if (next_time == 0) {
      next_time = millis() + timer_millis;
      return false;
    }
    int now = millis();
    if (next_time < now) {
      next_time += timer_millis; 
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
  set_RGB_LED(1,0,0);

  // est.set_gravity();

  // set_RGB_LED(1,0,1);

}

Timer timerA(100); // 100 millis
Timer timerB(200);  // 1000 millis
int i = 0;

void loop() {
  // est.imu.update();

  // if (timerA.run()) {
  //   est.update();
  //   Serial.print("Integrations: ");
  //   Serial.println(est.update_counter);
  //   Serial.print("Reads: ");
  //   Serial.println(est.imu.read_counter);
  //   Serial.print("Preintegrations: ");
  //   Serial.println(est.imu.integrate_counter);
  //   est.print();
  //   est.imu.print();
  //   est.imu.print_raw();
  // }

  if (timerB.run()) {
    i = min(1000,i+1);
    thrust.update({i/100.0f,0.0f,0.0f,0.0f});
    Serial.println(i);
    if(i < 100){
      set_RGB_LED(1,0,0);
    } else if (i < 200) {
      set_RGB_LED(1,0,1);
    } else if (i < 300) {
      set_RGB_LED(0,0,1);
    } else if (i < 400) {
      set_RGB_LED(0,1,1);
    } else if (i < 500) {
      set_RGB_LED(0,1,0);
    } else if (i < 600) {
      set_RGB_LED(1,1,0);
    } else if (i < 700) {
      set_RGB_LED(1,1,1);
    } else if (i < 800) {
      set_RGB_LED(0,0,0);
    }
  }

}