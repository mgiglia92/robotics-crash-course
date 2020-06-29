#include <motor_control.h>
#include <MPU6050.h>
#include <HC_SR04.h>

//Global Variables
MPU6050 sensor(4, 5);
HC_SR04 distance_sensor(10, 2, 0, 200);

void setup() {
  // put your setup code here, to run once:
  sensor.initialize(); //initialize MPU6050
  sensor.calibrate(); //calibrate MPU6050
  distance_sensor.begin(); 
  distance_sensor.start();
  Serial.begin(9600);
}

void loop() {
//  sensor.update();
//  float value = sensor.get_accel('z');
//  Serial.println(value);
    if(distance_sensor.isFinished())
    {
      int distance = distance_sensor.getRange();
      Serial.println(distance);
      distance_sensor.start();
    }
}
