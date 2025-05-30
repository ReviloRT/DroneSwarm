#include "imu.hpp"


void print_TF3(TMF3 &v) {
    Serial.printf("V:[%f, %f, %f]\n", v(0), v(1), v(2));
}

void print_TF3(TF3 &v) {
    Serial.printf("V:[%f, %f, %f]\n", v(0), v(1), v(2));
}

void print_TF3x3(TF3x3 &m) {
    Serial.printf("M:[\n[%f, %f, %f]\n[%f, %f, %f]\n[%f, %f, %f]\n] \n", m(0,0), m(0,1), m(0,2), m(1,0), m(1,1), m(1,2), m(2,0), m(2,1), m(2,2));
}

void Preintegrator::update() {
    _read();
    _preintegrate();
}

void Preintegrator::zero_integral() {
    Delta_t = 0;
    Delta_p.zeros();
    Delta_v.zeros();
    Delta_R.eye();
}

auto Preintegrator::_rot_exp(TMF3 &vect,float dt) {
    TF3x3 eye;
    eye.eye(); 

    float mag = Fastor::norm(vect);
    TF3x3 skew_symmetric(0);
    
    skew_symmetric(0,1) = -vect(2)/mag;
    skew_symmetric(0,2) =  vect(1)/mag;
    skew_symmetric(1,0) =  vect(2)/mag;
    skew_symmetric(1,2) = -vect(0)/mag;
    skew_symmetric(2,0) = -vect(1)/mag;
    skew_symmetric(2,1) =  vect(0)/mag;

    // Serial.println("_rot_exp");
    // Serial.println(dt);
    // Serial.println(mag);
    // print_TF3x3(skew_symmetric);
    // Serial.println(sin(mag*dt));
    // Serial.println((1-cos(mag*dt)));
    TF3x3 ret = eye + sin(mag*dt)*skew_symmetric + (1-cos(mag*dt))* (skew_symmetric % skew_symmetric);
    // print_TF3x3(ret);
    
    return ret;
}

void Preintegrator::_preintegrate() {
    if (_this_sample_t == _prev_sample_t) return;

    // Serial.println("Sample");
    // print_TF3(_sample_accel);
    // print_TF3(_sample_omega);

    float del_t = (float)(_this_sample_t - _prev_sample_t)*(0.000001);
    auto del_R =_rot_exp(_sample_omega,del_t);
    auto del_p = del_t*del_t*0.5*_sample_accel;
    auto del_v = del_t*_sample_accel;
    

    // Serial.printf("dels, dt: %f\n",del_t);
    // print_TF3(del_p);
    // print_TF3(del_v);
    // print_TF3x3(del_R);

    Delta_t += del_t;
    Delta_p = evaluate(Delta_p + Delta_v*del_t + Delta_R % del_p);
    Delta_v = evaluate(Delta_v + Delta_R % del_v);
    Delta_R = evaluate(Delta_R % del_R);

    // Serial.println("Deltas");
    // print_TF3(_Delta_p);
    // print_TF3(Delta_v);
    // print_TF3x3(Delta_R);

    _prev_sample_t = _this_sample_t;
    integrate_counter ++;
}


TF3 Preintegrator::get_accelerations() {
    return TF3(_sample_accel);
}

void Preintegrator::print() {
    Serial.printf(">dt:%f\n", Delta_t);
    Serial.printf(">dpx:%f\n",Delta_p(0));
    Serial.printf(">dpy:%f\n",Delta_p(1));
    Serial.printf(">dpz:%f\n",Delta_p(2));
    Serial.printf(">dvx:%f\n",Delta_v(0));
    Serial.printf(">dvy:%f\n",Delta_v(1));
    Serial.printf(">dvz:%f\n",Delta_v(2));
    Serial.printf("dR:[\n[%f, %f, %f]\n[%f, %f, %f]\n[%f, %f, %f]\n] \n", Delta_R(0,0), Delta_R(0,1), Delta_R(0,2), Delta_R(1,0), Delta_R(1,1), Delta_R(1,2), Delta_R(2,0), Delta_R(2,1), Delta_R(2,2));
}





void BMI323::init() {
    Wire.begin(PIN_SDA,PIN_SCL,1000000);
    
    _writeRegister16(0x7E, 0xDEAF);
    delay(50);    
    /*
    * Acc_Conf P.91
    * mode:        0b111  -> High performance
    * average:     0b110  -> 64 samples
    * filtering:   0b1    -> ODR/4
    * range:       0b000  -> 2g
    * ODR:         0b1101 -> 3.2kHz
    * Total:       0b0111011010001101
    *              0x768D
    */
    _writeRegister16(0x20,0x768D);//Setting accelerometer  
    /*
    * Gyr_Conf P.93
    * mode:        0b111  -> High performance
    * average:     0b110  -> 64 samples
    * filtering:   0b1    -> ODR/4
    * range:       0b000  -> 2g
    * ODR:         0b1101 -> 3.2kHz
    * Total:       0b0111011010001101
    *              0x768D
    */
    _writeRegister16(0x21,0x768D);//Setting gyroscope  

    _prev_sample_t = _this_sample_t;
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
void BMI323::_read()  {
    // if(_readRegister16(0x02) != 0x00) return; // Nothing Ready
    Wire.beginTransmission(BMI323_ADR);
    Wire.write(0x03);
    Wire.endTransmission();
    int n = Wire.requestFrom(BMI323_ADR, 20);   // 7 registers, each 16 bits, plus 2 dummy bytes
    if( n != 20) return; // the I2C transaction was a failure!

    // Get rid of dummy bytes
    Wire.read(); 
    Wire.read();

    uint16_t ret[18];
    // Serial.print("Ret: [");
    for (size_t i = 0; i < 18; i++){
        ret[i] = Wire.read();
        // Serial.print(ret[i]);
        // Serial.print(",");
    }
    // Serial.print("]\n");

    const float accel_scale = 16.38/65535.0*2;
    const float omega_scale = 262.144/65535.0/180*PI;

    _accels[0] = static_cast<float>((static_cast<int16_t>(ret[1] << 8)) | ret[0])*(accel_scale);
    _accels[1] = static_cast<float>((static_cast<int16_t>(ret[3] << 8)) | ret[2])*(accel_scale);
    _accels[2] = static_cast<float>((static_cast<int16_t>(ret[5] << 8)) | ret[4])*(accel_scale);
    _omegas[0] = static_cast<float>((static_cast<int16_t>(ret[7] << 8)) | ret[6])*(omega_scale) + 0.0055;
    _omegas[1] = static_cast<float>((static_cast<int16_t>(ret[9] << 8)) | ret[8])*(omega_scale) + 0.001;
    _omegas[2] = static_cast<float>((static_cast<int16_t>(ret[11] << 8)) | ret[10])*(omega_scale) + 0.005;

    _temp = static_cast<float>((static_cast<int16_t>(ret[13] << 8)) | ret[12])/512.0 + 23.0;

    _this_sample_t =  ret[14]; 
    _this_sample_t |= ret[15] << 8; 
    _this_sample_t |= static_cast<uint32_t>(ret[16]) << 16; 
    _this_sample_t |= static_cast<uint32_t>(ret[17]) << 24; 
    _this_sample_t *= 39.0625;

    read_counter ++;
    
}


void BMI323::print_raw() {
    Serial.print("BMI323 Raw: ");
    Serial.print(">rt:");
    Serial.println(_this_sample_t);
    Serial.print(">rax:");
    Serial.println(_accels[0]);
    Serial.print(">ray:");
    Serial.println(_accels[1]);
    Serial.print(">raz:");
    Serial.println(_accels[2]);
    Serial.print(">rgx:");
    Serial.println(_omegas[0]);
    Serial.print(">rgy:");
    Serial.println(_omegas[1]);
    Serial.print(">rgz:");
    Serial.println(_omegas[2]);
}

