#include <motor_control.h>
#include <HC_SR04.h>
#include <SODAR.h>
#include <State.h>
#include <PID_v1.h>
#include <MPU6050.h>
#include <motor_control.h>

// Servo Pin declaration
#define SERVO_PIN 9  //servo connect to D9

//Ultrasonic distance sensor pin declaration
#define TRIG_PIN 10
#define ECHO_PIN 2
#define ECHO_INT 0
#define CENTER 91
#define RESOLUTION 10
#define UPPER_RANGE 111
#define LOWER_RANGE 71

//Global Variables
SODAR mysodar(TRIG_PIN, ECHO_PIN, ECHO_INT, SERVO_PIN, 91, 10, 131, 51);
MPU6050 accel(4, 5);
State control_state;
//PID control gains
double kp = 2;
double ki = 0;
double kd = 1;
double output;
double input;
double setpoint;
PID controls(&input, &output, &setpoint, kp, ki, kd, DIRECT);

void setup()
{
  //Initialize accelerometer
  accel.initialize();
  accel.calibrate();
  
  //Initialize SODAR
  mysodar.initialize_arrays();
  mysodar.init();

  //Set some PID class parameters
  controls.SetMode(1);
  controls.SetOutputLimits(-100, 100);
  controls.SetSampleTime(10);

  //Set linear state for movement
  control_state.setLinearState(155);
  
  //Begin serial comms
  Serial.begin(9600);
}

void loop()
{
  //update sodar
//  mysodar.update();
  //update accelerometer
  accel.update();
  //PID input from accelerometer
  input = accel.get_ang_vel('z');
  //update PID setpoint
  setpoint = control_state.getRotationState();
  //Compute PID controls
  controls.Compute();
  //Set motor power based on feedback from controls PID class
  raw_motor_control(control_state.getLinearState()-output, control_state.getLinearState()+output);
//  raw_motor_control(control_state.getLinearState(), control_state.getLinearState());
  
  //Print
  Serial.println(input);
}
