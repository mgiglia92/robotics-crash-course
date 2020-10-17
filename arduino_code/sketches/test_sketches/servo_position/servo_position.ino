#include <MPU6050.h>
#include <Servo.h>

#define SDA 4
#define SCL 5

Servo actuator;
MPU6050 IMU(SDA, SCL);

void setup(){
  actuator.attach(9);
  IMU.initialize();
  IMU.calibrate();
  Serial.begin(9600);
}

void loop(){
  IMU.update();
  int val = IMU.get_accel('y')*90;
  
  actuator.write(90+val);
  Serial.println(val);
  
}
