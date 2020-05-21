#include<Wire.h>
const float gyro_sensitivity = 131;
const float accel_sensitivity = 16384;
const int MPU6050_addr=0x68;
//int16_t AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;
void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  Serial.begin(9600);

  //configure gyro
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x1B);
  Wire.write(0b0000000);
  Wire.endTransmission();

  //configure accel
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x1C);
  Wire.write(0b00000000);
  Wire.endTransmission();
}
void loop(){
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);
  float AccX=(float)(Wire.read()<<8|Wire.read());
  int16_t AccY=Wire.read()<<8|Wire.read();
  int16_t AccZ=Wire.read()<<8|Wire.read();
  int16_t Temp=Wire.read()<<8|Wire.read();
  int16_t GyroX=Wire.read()<<8|Wire.read();
  int16_t GyroY=Wire.read()<<8|Wire.read();
  int16_t GyroZ=Wire.read()<<8|Wire.read();
  Serial.print("AccX = "); Serial.println(AccX);

  delay(100);
}
