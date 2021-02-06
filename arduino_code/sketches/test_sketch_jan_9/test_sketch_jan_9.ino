#include <motor_control.h>
#include <HC_SR04.h>
#include <MPU6050.h>

//Global variable scope
HC_SR04 sensor(10, 2, 0);
double distance;

void setup() {
// Initialize the distance sensor
sensor.begin();

// Start the first reading of the sensor
sensor.start(); 

//Start serial comms for debugging
Serial.begin(9600);
}

void loop() {
  //Check if the sensor has finished detecting distance
  if(sensor.isFinished())
  {
    //Get the distance saved in the sensor class
    distance = sensor.getRange();

    // Restart the sensor to measure the next distance
    sensor.start();
  }

  //Print the distance value to the serial monitor
  Serial.print("dist: "); Serial.println(distance);
}
