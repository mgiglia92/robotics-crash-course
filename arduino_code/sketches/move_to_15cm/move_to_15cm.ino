#include <motor_control.h>
#include <HC_SR04.h>

//Global variable scope
HC_SR04 sensor(10, 2, 0);
double distance;

//--------------------------------------------------------------------
void setup() {
  // Initialize the distance sensor
  sensor.initialize();
  
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
 //subroutine 1: Move forward until distance < 15cm
 while(distance > 15){
    //set motors to drive forward
    raw_motor_control(150,150);

    //update distance variable from distance sensor
    distance = sensor.getRangeReset();
 }

 //subroutine 2: move backward until distance >30cm
 while(distance < 30){
    //set motors to drive forward
    raw_motor_control(-150,-150);

    //update distance variable from distance sensor
    distance = sensor.getRangeReset();
 }

 //subroutine 3: Rotate CCW until distance > 50cm
 while(distance < 50){
    //set motors to drive forward
    raw_motor_control(-150,150);

    //update distance variable from distance sensor
    distance = sensor.getRangeReset();
 }

 //End of loop, repeat!
}
