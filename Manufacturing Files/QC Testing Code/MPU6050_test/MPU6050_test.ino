/*
 * Test to see if sensor is working, connect sensor and run, should get 
 * reasonable values in serial monitor
 */
 #include <MPU6050.h>

 MPU6050 IMU(4, 5);

void setup()
{
  //Initialization procedure for MPU6050
  IMU.initialize();
  //Calibrate sensor
//  IMU.calibrate();

  //Start serial comms
  Serial.begin(9600);
}

void loop()
{
  //Update sensor
  IMU.update();

  //Print value to screen
  Serial.print("ax: "); Serial.println(IMU.get_accel('x'));
}
