/* Basic's of using an arduino. This will cover digitalWrite(), analogWrite()[using pwm], analogRead()
 * Blink/Dim LED Example
 * Read accelerometer, control brightness with accel angle
 * How to use a Servo
 */

//Important pin definitions
#define SERVOPIN 9 //Needs pwm
#define ACCELPIN 0 //Will be an Analog pin
#define LEDPIN 3 //Needs pwm
#define ACCELMAX 1.05 //Maximum acceleration sensor can output
#define ACCELMIN -1.05 //Minimum accel sensor can output
#define SDA 4 //Data pin for accelerometer
#define SCL 5 //Clock pin for accelerometer

//Import libraries
#include <Servo.h>
#include <MPU6050.h>
//TODO: Add accelorometer library to get data from accelerometer

//Global variables
float ax; //accelerometer value x-axis
float ay; //y-axis
float az; //z-axis
Servo servo;
int angle; // variable that will control servo position
MPU6050 sensor(SDA, SCL); //Accelerometer class


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

  //Calibrate sensor and initialize sensor
  sensor.initialize();
  sensor.calibrate();

  //Start serial communication for debugging
  Serial.begin(9600);
}

void loop() {

  //Read accelerometer values
  sensor.update();
  //get x axis acceleration
  ax = sensor.get_accel('x');
  //Convert raw accelerometer values to usable LED brightness values  
  angle = raw_accel_to_angle(ax);
  servo.write((int)angle); //NOTE: we use analog write instead of digitalwrite when we want to use PWM on a digital pin.
  Serial.println(ax);
}

//Function to convert raw accel values to LED brightness values
float raw_accel_to_angle(float raw)
{
  //Linearly map the raw values to the brightneess values
  //NOTE: LED brightness must be a number between 0-180, since sevo is controlled with a value 0-180
  return mapf(raw, ACCELMIN, ACCELMAX, 180, 0);
}

double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
