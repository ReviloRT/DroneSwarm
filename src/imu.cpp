#include "imu.hpp"


void Preintegrator::update() {
    _read();
    _preintegrate();
}

float Preintegrator::get_t() {
    return _Delta_t;
}

TF3 Preintegrator::get_p() {
    return _Delta_p;
}

TF3 Preintegrator::get_v() {
    return _Delta_v;
}

TF3 Preintegrator::get_R() {
    return _Delta_R;
}
void Preintegrator::zero_integral() {
    _Delta_t = 0;
    _Delta_p.zeros();
    _Delta_v.zeros();
    _Delta_R.zeros();
}

void Preintegrator::_preintegrate() {
    uint64_t now = micros();
    float del_t = _this_sample_t - _prev_sample_t;
}

void Preintegrator::print() {
    Serial.printf("IMU: t, a(x,y,z), g(x,y,z) -> %f, %f, %f, %f, %f, %f, %f\n",
        _this_sample_t, _readings[0],_readings[1],_readings[2],_readings[3],_readings[4],_readings[5]);
    Serial.printf("Preint: Dt, Dp, Dv, DR -> %f,",_Delta_t);
    Serial.printf("[%f, %f, %f ], ",_Delta_p(0),_Delta_p(1),_Delta_p(2));
    Serial.printf("[%f, %f, %f ], ",_Delta_v(0),_Delta_v(1),_Delta_v(2));
    Serial.printf("[%f, %f, %f ]\n",_Delta_R(0),_Delta_R(1),_Delta_R(2));
}




void BMI323::init() {
    Wire.begin(PIN_SDA,PIN_SCL,400000);
    
    _writeRegister16(0x7E, 0xDEAF);
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
    _writeRegister16(0x20,0x708B);//Setting accelerometer  
    /*
    * Gyr_Conf P.93
    * mode:        0x7000  -> High
    * average:     0x0000  -> No
    * filtering:   0x0080  -> ODR/4
    * range:       0x0000  -> 125kdps
    * ODR:         0x000B  -> 800Hz
    * Total:       0x708B
    */
    _writeRegister16(0x21,0x708B);//Setting gyroscope  
}


//Write data in 16 bits
void BMI323::_writeRegister16(uint16_t reg, uint16_t value) {
  Wire.beginTransmission(BMI323_ADR);
  Wire.write(reg);
  //Low 
  Wire.write((uint16_t)value & 0xff);
  //High
  Wire.write((uint16_t)value >> 8);
  Wire.endTransmission();
}

//Read data in 16 bits
uint16_t BMI323::_readRegister16(uint8_t reg) {
  Wire.beginTransmission(BMI323_ADR);
  Wire.write(reg);
  Wire.endTransmission(false);
  int n = Wire.requestFrom(BMI323_ADR, 4);  
  uint16_t data[20];
  int i =0;
  while(Wire.available()){
    data[i] = Wire.read();
    i++;
  }  
  return (data[3]   | data[2] << 8);
}


//Read all axis
void BMI323::_read() {
    if(_readRegister16(0x02) == 0x00) {
        Wire.beginTransmission(BMI323_ADR);
        Wire.write(0x03);
        Wire.endTransmission();
        Wire.requestFrom(BMI323_ADR, 14);
        uint8_t response[14];
        int i = 0;
        while(Wire.available()){
            response[i] = Wire.read();
            i++;
        }

        //offset of 2 because the 2 first bytes are dummy (useless)  
        _readings[0] = (response[2]   | (uint16_t)response[3] << 8);  //0x03
        _readings[1] = (response[4]   | (uint16_t)response[5] << 8);  //0x04
        _readings[2] = (response[6]   | (uint16_t)response[7] << 8);  //0x05
        _readings[3] = (response[8]   | (uint16_t)response[9] << 8);  //0x06
        _readings[4] = (response[10]  | (uint16_t)response[11] << 8); //0x07
        _readings[5] = (response[12]  | (uint16_t)response[13] << 8); //0x08
    }
}


