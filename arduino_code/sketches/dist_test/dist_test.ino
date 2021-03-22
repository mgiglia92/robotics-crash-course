#include <motor_control.h>
#include <HC_SR04.h>

//Global variable scope
HC_SR04 sensor(10, 2, 0);
double distance;
double threshold = 15; //threshold distance from object to trigger motion

//--------------------------------------------------------------------
void setup() {
  // Initialize the distance sensor
  sensor.begin();
  
  // Start the first reading of the sensor
  sensor.start(); 
  
  // Wait till sensor has first distance reading
  while(sensor.isFinished() == false){}
  
  // Sensor has reading, update distance value
  distance = sensor.getRange();
  
  //Start serial comms for debugging
  Serial.begin(9600);
}

//----------------------------------------------------------------------
void loop() {
  //Subroutine 1: Drive forward till distance < threshold while updating distance variable
  while(distance > threshold)
  {
    // Drive forward
    raw_motor_control(155, 155);

    // If sensor has new reading update distance variable
    if(sensor.isFinished() == true)
    {
      distance = sensor.getRange();

      //Restart distance sensor for a new reading
      sensor.start();
    }
  }
  
  //Print the distance value to the serial monitor
  Serial.print("dist: "); Serial.print(distance);
}
