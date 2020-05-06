/* Basic's of using an arduino. This will cover digitalWrite(), analogWrite()[using pwm], analogRead()
 * Blink/Dim LED Example
 * Read accelerometer, control brightness with accel angle
 * How to use a Servo
 */

//Important pin definitions
#define SERVOPIN 9 //Needs pwm
#define ACCELPIN 0 //Will be an Analog pin
#define LEDPIN 3 //Needs pwm
#define ACCELMAX 10 //Maximum acceleration sensor can output
#define ACCELMIN -10 //Minimum accel sensor can output
#define SDA 4 //Data pin for accelerometer
#define SCL 5 //Clock pin for accelerometer

//Import libraries
#include <Servo.h>
#include <GY_521.h>
//TODO: Add accelorometer library to get data from accelerometer

//Global variables
int ax; //accelerometer value x-axis
int ay; //y-axis
int az; //z-axis
Servo servo;
int brightness; // variable that will control LED brightness
GY_521 sensor(SDA, SCL); //Accelerometer class


void setup() {
  //Setup pins as INPUT or OUTPUT
  pinMode(SERVOPIN, OUTPUT); //Servo pin sends voltage to servo, so it's an OUTPUT
  pinMode(LEDPIN, OUTPUT); //LED pin sends voltage to the LED, so it's an OUTPUT

  //Initialize servo by attaching it to the correct pin
  servo.attach(SERVOPIN);
  //Do a couple movements to practice using the servo
  servo.write(0);
  delay(200);
  servo.write(45);
  delay(200);
  servo.write(90);
  delay(200);
  
}

void loop() {

  //Read accelerometer values
  sensor.update();
  //TODO: Setup library to read values
  ax = sensor.get_accel('x');
  //Convert raw accelerometer values to usable LED brightness values  
  brightness = raw_accel_to_brightness(ax);
  analogWrite(LEDPIN, brightness); //NOTE: we use analog write instead of digitalwrite when we want to use PWM on a digital pin.
}

//Function to convert raw accel values to LED brightness values
int raw_accel_to_brightness(float raw)
{
  //Linearly map the raw values to the brightneess values
  //NOTE: LED brightness must be a number between 0-255, since PWM is represented with an 8bit number, so 0%-0, 100%-255
  return map(raw, ACCELMIN, ACCELMAX, 0, 255);
}
