#include <motor_control.h>
#include <HC_SR04.h>
#include <Servo.h>

//Global variable scope
HC_SR04 sensor(10, 2, 0);
double distance;
Servo servo; //Instantiate a servo variable
double distace_center;
double distance_left;
double distance_right;

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

  //Connect the servo to the digital pin it is connected to
  servo.attach(9);
  servo.write(90);
  
  //Start serial comms for debugging
  Serial.begin(9600);
}

//----------------------------------------------------------------------
void loop() {
 //subroutine 1: Move forward until distance < 15cm
 while(distance > 15){
    //set motors to drive forward
    raw_motor_control(200,200);

    //update distance variable from distance sensor
    distance = sensor.getRangeReset();
 }

 //subroutine 2: Do a "scan" and turn appropriately
    //stop moving
    raw_motor_control(0,0);

    //Face sensor to the left
    servo.write(135);
    //wait some time for servo to get to position
    delay(300);
    //Get left distance reading
    distance_left = sensor.getRangeReset(); //Purge the old distance reading
    delay(100);
    distance_left = sensor.getRangeReset();

    //Face sensor right
    sevo.write(45);
    delay(300);
    distance_right = sensor.getRangeReset();    //Purge the old distance reading
    delay(100);
    distance_right = sensor.getRangeReset(); 

    
    //Determine direction to rotate
    if(distance_right > distance_left) { //turn right }
      raw_motor_control(turn right);
      delay(500);
      raw_motor_control(0,0);
    }
    if(distance_left > distance_right) { //turn left
      raw_motor_control(turn left);
      delay(500);
      raw_motor_control(0,0);
    }

 //subroutine 3: Rotate CCW until distance > 50cm
 while(distance < 100){
    //set motors to drive forward
    raw_motor_control(-200,200);

    //update distance variable from distance sensor
    distance = sensor.getRangeReset();
 }

 //End of loop, repeat!
}
