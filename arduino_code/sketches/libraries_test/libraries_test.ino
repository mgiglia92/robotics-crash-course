#include <motor_control.h>
#include <HC_SR04.h>
#include <MPU6050.h>

//Global variable scope
HC_SR04 sensor(10, 2, 0);
MPU6050 IMU(4, 5);
double z;
double distance;

//--------------------------------------------------------------------
void setup() {
// Initialize the distance sensor
sensor.begin();

// Start the first reading of the sensor
sensor.start(); 

// Initialize the IMU (MPU6050)
IMU.initialize();

//Start serial comms for debugging
Serial.begin(9600);
}

//----------------------------------------------------------------------
void loop() {
  //Update IMU each time the loop starts
  IMU.update();
  // Get the z acceleration from the sensor
  z = IMU.get_accel('x');
  
  //Check if the sensor has finished detecting distance
  if(sensor.isFinished())
  {
    //Get the distance saved in the sensor class
    distance = sensor.getRange();

    // Restart the sensor to measure the next distance
    sensor.start();
  }

  //Raw motor control test
  raw_motor_control(100, 100);

  //Print the distance value to the serial monitor
  Serial.print("dist: "); Serial.print(distance);
  Serial.print(" | z grav: "); Serial.println(z);
}
