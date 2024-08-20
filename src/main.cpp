#include <Arduino.h>
#include <Wire.h>

#include "led.hpp"
#include "rotors.hpp"

Rotors rotors;


#define PIN_LED GPIO_NUM_2
#define BMI323_ADDR 0x68
#define PIN_SDA GPIO_NUM_35
#define PIN_SCL GPIO_NUM_36

struct BMI323Data {
  uint16_t x, y, z;
  uint16_t gyr_x, gyr_y, gyr_z;
  float temperatureInDegree;
};

void blink(uint8_t pin, uint32_t millis);
void init_bmi323();
void writeRegister16(uint16_t reg, uint16_t value);
uint16_t readRegister16(uint8_t reg);
BMI323Data readAllAccel();

void setup() {
  init_leds();

  delay(1000);
  rotors.on(128);
  Serial.begin(115200);
  Wire.begin(PIN_SDA,PIN_SCL,400000);
  digitalWrite(PIN_LED,HIGH);

  init_bmi323();
}

void loop() {
  if(readRegister16(0x02) == 0x00) {
    //Read ChipID
    // Serial.print("ChipID:");
    // Serial.print(readRegister16(0x00));    
    BMI323Data data = readAllAccel();             // read all accelerometer/gyroscope/temperature data     
    Serial.print(">x:");
    Serial.println(data.x);
    Serial.print(">y:");
    Serial.println(data.y);
    Serial.print(">z:");
    Serial.println(data.z);
    Serial.print(">gyr_x:");
    Serial.println(data.gyr_x);
    Serial.print(">gyr_y:");
    Serial.println(data.gyr_y);
    Serial.print(">gyr_z:");
    Serial.println(data.gyr_z);
    Serial.print(">temp:");
    Serial.println(data.temperatureInDegree);    
  }
  
}

void blink(uint8_t pin, uint32_t millis) {
  digitalWrite(pin,HIGH);
  delay(millis);
  digitalWrite(pin,LOW);
  delay(millis);
}

void init_bmi323() {
  writeRegister16(0x7E, 0xDEAF);
  delay(50);    
  /*
   * Acc_Conf P.91
   * mode:        0x7000  -> High
   * average:     0x0000  -> No
   * filtering:   0x0080  -> ODR/4
   * range:       0x0000  -> 2G
   * ODR:         0x000B  -> 800Hz
   * Total:       0x708B
   */
  writeRegister16(0x20,0x708B);//Setting accelerometer  
  /*
   * Gyr_Conf P.93
   * mode:        0x7000  -> High
   * average:     0x0000  -> No
   * filtering:   0x0080  -> ODR/4
   * range:       0x0000  -> 125kdps
   * ODR:         0x000B  -> 800Hz
   * Total:       0x708B
   */
  writeRegister16(0x21,0x708B);//Setting gyroscope  
}

//Write data in 16 bits
void writeRegister16(uint16_t reg, uint16_t value) {
  Wire.beginTransmission(BMI323_ADDR);
  Wire.write(reg);
  //Low 
  Wire.write((uint16_t)value & 0xff);
  //High
  Wire.write((uint16_t)value >> 8);
  Wire.endTransmission();
}

//Read data in 16 bits
uint16_t readRegister16(uint8_t reg) {
  Wire.beginTransmission(BMI323_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  int n = Wire.requestFrom(BMI323_ADDR, 4);  
  uint16_t data[20];
  int i =0;
  while(Wire.available()){
    data[i] = Wire.read();
    i++;
  }  
  return (data[3]   | data[2] << 8);
}

//Read all axis
BMI323Data readAllAccel() {
  Wire.beginTransmission(BMI323_ADDR);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.requestFrom(BMI323_ADDR, 20);
  uint16_t response[20];
  int i =0;
  while(Wire.available()){
    response[i] = Wire.read();
    i++;
  }

  //Offset = 2 because the 2 first bytes are dummy (useless)
  int offset = 2;  
  BMI323Data data;
  data.x =             (response[offset + 0]   | (uint16_t)response[offset + 1] << 8);  //0x03
  data.y =             (response[offset + 2]   | (uint16_t)response[offset + 3] << 8);  //0x04
  data.z =             (response[offset + 4]   | (uint16_t)response[offset + 5] << 8);  //0x05
  data.gyr_x =         (response[offset + 6]   | (uint16_t)response[offset + 7] << 8);  //0x06
  data.gyr_y =         (response[offset + 8]   | (uint16_t)response[offset + 9] << 8);  //0x07
  data.gyr_z =         (response[offset + 10]  | (uint16_t)response[offset + 11] << 8); //0x08
  uint16_t temperature =   (response[offset + 12]  | (uint16_t)response[offset + 13] << 8); //0x09
  data.temperatureInDegree = (temperature/512.f) + 23.0f;  
  return data;
}