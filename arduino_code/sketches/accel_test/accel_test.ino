#include "MPU6050.h"

MPU6050 IMU(4, 5);

void setup() {
  // put your setup code here, to run once:
  IMU.initialize();
IMU.calibrate();
Serial.begin(9600);
Serial.print("444444");
}

void loop() {//update sensor
//  do_full();
bool val = IMU.update();
//Serial.print("a sen: "); Serial.println(IMU.accel_sensitivity);
//Serial.print("ax: "); Serial.print(IMU.get_accel('x')); 
Serial.print(" wz: "); Serial.println(IMU.get_ang_vel('z'));

//Serial.println(x/16000.0);

}
