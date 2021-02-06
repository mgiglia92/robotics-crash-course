// This will demonstrate the MPU6050
# include <MPU6050.h>

MPU6050 IMU(4,5);

void setup() {
  // Initializes and calibrates the MPU6050
  IMU.initialize();
//  IMU.calibrate();
  
  Serial.begin(115200);
}

void loop() {
  // Gets new readings from sensor
  IMU.update();

  // Replace this with other functions to get different information from the sensor
  Serial.print("accel. x: ");
  Serial.println(IMU.get_accel('x'));
}
