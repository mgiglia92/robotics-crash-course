#include <PID_v1.h>
#include <Servo.h>
#include <HC_SR04.h>
#include <motor_control.h>

#define SERVO_PIN 9
#define TRIG_PIN 10
#define ECHO_PIN 2
#define ECHO_INT 0
#define CENTER 90

Servo actuator;
HC_SR04 dist_sensor(TRIG_PIN, ECHO_PIN, ECHO_INT);

//PID parameters
double kp = 20;
double ki = 0;
double kd = 0;
double input;
double output;
double setpoint = 80;
PID keep_distance(&input, &output, &setpoint, kp, ki, kd, REVERSE);

void setup()
{
  //Begin serail comms for debug
  Serial.begin(9600);

  //Attach servo to servo pin, and rotate to point sensor forward
  actuator.attach(SERVO_PIN);
  actuator.write(CENTER);

  //Set PID hyperparams
  keep_distance.SetMode(1);
  keep_distance.SetSampleTime(50);
  keep_distance.SetOutputLimits(-200, 200);

  //Initialize distance sensor and start pinging for distances
  dist_sensor.begin();
  dist_sensor.start();
}

void loop()
{
  if(dist_sensor.isFinished())
  {
    input = dist_sensor.getRange();
    dist_sensor.start();
  }
  
  keep_distance.Compute();

  raw_motor_control(output*1.1, output);

  Serial.print("dist: "); Serial.print(input); Serial.print(" output: "); Serial.println(output);
}
