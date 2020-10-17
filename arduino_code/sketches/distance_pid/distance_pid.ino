#include <HC_SR04.h>
#include <PID_v1.h>
#include <motor_control.h>
#include <Servo.h>

#define TRIG_PIN 10
#define ECHO_PIN 2
#define ECHO_INT 0

//Global variables
HC_SR04 sensor(TRIG_PIN, ECHO_PIN, ECHO_INT);
Servo actuator;

//Setup PID controller variables
double setpoint = 80;
double input;
double output;
double kp = 20;
double ki = 0;
double kd = 2;
PID controller(&input, &output, &setpoint, kp, ki, kd, REVERSE);

void setup() {
 //setup motors
 motor_setup();

 //setup servo and point forward
 actuator.attach(9);
 actuator.write(90); //points the servo forward

 //initialize distance sensor
 sensor.begin();
 sensor.start();

 //Set PID parameters
 controller.SetOutputLimits(-255, 255);
 controller.SetSampleTime(25);
 controller.SetMode(1);

 //begin serial for debuggin
 Serial.begin(9600);

}

void loop() {
  //get distance value from sensor, update input to be distance value
  if(sensor.isFinished())
  {
    input = sensor.getRange();
    sensor.start(); //restart sensor 
  }
  
  //compute PID controller once input has been updated
  controller.Compute();
  
  //set motor power based on output from PID controller
  raw_motor_control(1.1*output, output);

  Serial.print("output: "); Serial.print(output); Serial.print(" distance: "); Serial.println(input);
  
}
