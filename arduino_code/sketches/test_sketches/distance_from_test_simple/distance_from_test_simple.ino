#include <Servo.h>
#include <HC_SR04.h>
#include <motor_control.h>

#define SERVO_PIN 9
#define TRIG_PIN 10
#define ECHO_PIN 2
#define ECHO_INT 0
#define CENTER 90
#define DESIRED_DISTANCE 80

//Global Variables
Servo actuator;
HC_SR04 dist_sensor(TRIG_PIN, ECHO_PIN, ECHO_INT);
int distance; //Variable to save distance from distance sensor

void setup()
{
  //Begin serail comms for debug
  Serial.begin(9600);

  //Attach servo to servo pin, and rotate to point sensor forward
  actuator.attach(SERVO_PIN);
  actuator.write(90);

  //Initialize distance sensor and start pinging for distances
  dist_sensor.begin();
  dist_sensor.start();
}

void loop()
{
  if(dist_sensor.isFinished())
  {
    distance = dist_sensor.getRange();
    dist_sensor.start();
  }
  
  raw_motor_control(output*1.1, output);

  Serial.print("dist: "); Serial.print(input); Serial.print(" output: "); Serial.println(output);
}
