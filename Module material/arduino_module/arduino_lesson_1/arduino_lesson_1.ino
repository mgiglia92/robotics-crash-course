/* NOTE: Make sure you have changed the sketch book location.
 * Go to File -> Preferences
 * In the Sketchbook location textbox place the full directory to the arduino_code folder
 * Should be something like "D:\Documents\Robotics Crash Course\arduino_code" 
 */
 
/* Basic's of using an arduino. This will cover digitalWrite(), analogWrite()[using pwm], analogRead()
 * Read accelerometer 
 * How to use a Servo
 * Control servo angle with accelerometer data
 * 
 */

 /* Connect sensor shield to arduino
  * Using jumper wires connect MPU6050 to sensor shield
  * MPU6050 -> Sensor Shield
  * Vcc     -> +5V
  * GND     -> GND
  * SDA     -> D4
  * SCL     -> D5
  * 
  * Connect a servo horn to the servo motor
  * Connect the servo wiring harness to the sensor shield on D9
  * NOTE: Pay attention to the orientation when you plug it in, make sure
  * the proper colored wires are connected to the correct pins (+5V, GND, Signal)
  */

//Important pin definitions
#define SERVOPIN 9 //Needs pwm
#define SENSORMAX 1.05 //Maximum value sensor can output (important for scaling depending on if we use acceleration of angular velocity)
#define SENSORMIN -1.05 //Minimum value sensor can output
#define SDA 4 //Data pin for accelerometer
#define SCL 5 //Clock pin for accelerometer

//Import libraries
#include <Servo.h>
#include <MPU6050.h>

//Global variables
float ax; //accelerometer value x-axis
float ay; //y-axis
float az; //z-axis
float wx; //angular velocity x-axis
float wy; //angular velocity y-axis
float wz; //angular velocity z-axis
Servo servo;
double angle; // variable that will control servo position
MPU6050 accelerometer(SDA, SCL); //Accelerometer class


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

  //Read accelerometer values
  accelerometer.update();
  
  //get x axis acceleration
  ax = accelerometer.get_accel('x');

  //get z axis angular velocity
  wz = accelerometer.get_ang_vel('z');
  
  //Convert raw accelerometer values to usable servo angle value 
  angle = raw_accel_to_angle(ax);
  
  servo.write((int)angle); //(int) is called a "cast", it will take the variable angle and turn it into an integer if it is not
  Serial.println(ax);
}

//Function to convert raw accel values to LED brightness values
float raw_accel_to_angle(float raw)
{
  //Linearly map the raw values to the brightneess values
  //NOTE: angle be a number between 0-180, since sevo is controlled with a value 0-180
  return mapf(raw, SENSORMIN, SENSORMAX, 0, 180);
}

//mapf is very similar to the map() function, except it can use double datatypes instead of just integers
//This will linearly interpolate, or extrapolate the value passed in, based on the min and max vals passed in
double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
