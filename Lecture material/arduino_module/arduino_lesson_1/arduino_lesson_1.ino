/* Basic's of using an arduino. This will cover digitalWrite(), analogWrite()[using pwm], analogRead()
 * Blink/Dim LED Example
 * Read accelerometer, control brightness with accel angle
 * How to use a Servo
 */

//Important pin definitions
#define SERVOPIN 9 //Needs pwm
#define ACCELPIN 0 //Will be an Analog pin
#define LEDPIN 3 //Needs pwm

//Import libraries
#include <Servo.h>
//TODO: Add accelorometer library to get data from accelerometer

//Global variables
int ax; //accelerometer value x-axis
int ay; //y-axis
int az; //z-axis
Servo servo;
int brightness; // variable that will control LED brightness


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

}
