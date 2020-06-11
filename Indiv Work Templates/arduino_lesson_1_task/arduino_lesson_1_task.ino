/* Use arduino_lesson_1.ino as a base for this code
 * Don't be afraid to copy snippets of code that are relevant
 * for this task.
 * 
 * Task: In arduino_lesson_1.ino we used the acceleration value along
 * the x-axis to be the sensor value uesd to control the angle 
 * the servo will point towards.
 * In this task, instead of using the accelerometer value we will use
 * the angular_velocity along the z-axis to control the servo angle
 * HINT: Look at line 83 in arduino_lesson_1.ino for how to get
 * angular velocity values from the sensor
 */

//Important pin definitions
#define SERVOPIN 9 //Needs pwm
//Adjust SENSORMAX and SENSORMIN, to do so, just pring the values 
//you get for angular velocity z-axis, what are the minimum/maximum you get?
#define SENSORMAX 1.05 //Maximum acceleration accelerometer can output
#define SENSORMIN -1.05 //Minimum accel accelerometer can output
#define SDA 4 //Data pin for accelerometer
#define SCL 5 //Clock pin for accelerometer


//Included libraries
//add Servo Library
//add MPU6050 Library

//Global Variables
//Add global variables

void setup() {
  //Setup pins as INPUT or OUTPUT
  pinMode(SERVOPIN, OUTPUT); //Servo pin sends voltage to servo, so it's an OUTPUT
  
  //Initialize servo by attaching it to the correct pin
  servo.attach(SERVOPIN);
  
  //Do a couple movements to practice using the servo
  servo.write(0);
  delay(500);
  servo.write(45);
  delay(500);
  servo.write(90);
  delay(500);

  //Calibrate accelerometer and initialize accelerometer
  accelerometer.initialize();
  accelerometer.calibrate();

  //Start serial communication for debugging
  Serial.begin(9600);
}

void loop() {
 //Loop stuff here
}

//raw_ang_vel_to_accel function (copy and rename the function from arduino_lesson_1.ino)

//mapf function (don't be afraid to copy)
