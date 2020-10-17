#include <motor_control.h>
#include <MPU6050.h>

MPU6050 IMU(4,5);

void setup(){
  IMU.initialize();
  IMU.calibrate();
  IMU.update();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
IMU.update();
Serial.println(IMU.get_temp());
}
