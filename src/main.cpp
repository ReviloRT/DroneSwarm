#include <Arduino.h>
#include <Wire.h>

#include "led.hpp"
#include "rotors.hpp"

Rotors rotors;


#define PIN_LED GPIO_NUM_2
#define BMI323_ADDR 0x69
#define PIN_SDA GPIO_NUM_36
#define PIN_SCL GPIO_NUM_35

void blink(uint8_t pin, uint32_t millis);
void read_data();

void setup() {
  init_leds();

  delay(1000);
  rotors.on(128);
  Serial.begin(115200);
  Wire.begin(PIN_SDA,PIN_SCL,400000);
}

// void loop() {
//   blink(PIN_LED,250);
//   read_data();
// }

void blink(uint8_t pin, uint32_t millis) {
  digitalWrite(pin,HIGH);
  delay(millis);
  digitalWrite(pin,LOW);
  delay(millis);
}

void read_data() {
  Wire.beginTransmission(BMI323_ADDR);
  // Wire.write(0x03);
  int error = Wire.endTransmission();
  Serial.printf("Error %d\n",error);
  // int n_rec = Wire.requestFrom(BMI323_ADDR, 2);
  
  // if (n_rec < 2) return;

  // uint16_t rec[1] = {0};
  // for (size_t i = 0; i < 1; i++) {
  //   uint8_t b1 = Wire.read();
  //   uint8_t b2 = Wire.read();
  //   rec[i] = (b2 << 8) | b1;
  // }   
  
  // Serial.printf("%d, %d, %d ,%d, %d, %d\n",rec[0],rec[1],rec[2],rec[3],rec[4],rec[5]);

}


void loop() {
  byte error, address;
  int nDevices = 0;

  delay(5000);

  Serial.println("Scanning for I2C devices ...");
  for(address = 0x68; address < 0x6A; address++){
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0){
      Serial.printf("I2C device found at address 0x%02X\n", address);
      nDevices++;
    } else if(error != 2){
      Serial.printf("Error %d at address 0x%02X\n", error, address);
    }
  }
  if (nDevices == 0){
    Serial.println("No I2C devices found");
  }
}
