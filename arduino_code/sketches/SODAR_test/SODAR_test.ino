#include <motor_control.h>
#include <HC_SR04.h>
#include <SODAR.h>

// Servo Pin declaration
#define SERVO_PIN 9  //servo connect to D9

//Ultrasonic distance sensor pin declaration
#define TRIG_PIN 10
#define ECHO_PIN 2
#define ECHO_INT 0

//Global Variable
HC_SR04 dist_sensor();
SODAR mysodar(TRIG_PIN, ECHO_PIN, ECHO_INT, SERVO_PIN, 91, 10, 111, 71);

void setup()
{
  
}

void loop()
{
  
}
