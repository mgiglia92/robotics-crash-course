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
//Initialize SODAR class (not used currently)
SODAR mysodar(TRIG_PIN, ECHO_PIN, ECHO_INT, SERVO_PIN, 91, 10, 131, 51);

//Initialize accelerometer
MPU6050 accel(4, 5);

//Initailize state class to control forward/back, turn left turn right
State control_state; //State class to control car

//PID control gains
double kp = 2;
double ki = 0;
double kd = 0;
double output;
double input;
double setpoint;
PID drive_straight_controller(&input, &output, &setpoint, kp, ki, kd, DIRECT);

//Timer variables
unsigned long prev_time = millis();
unsigned long state_flip_period = 2000; //flip the control state every 1000 ms

//Base motor control parameters
int base_motor_power = 155;

void setup()
{
  //Initialize control_state
  control_state.setLinearState(155);
  control_state.setRotationState(45);
  
  //Initialize accelerometer
  accel.initialize();
  accel.calibrate();
  
  //Initialize SODAR
  mysodar.initialize_arrays();
  mysodar.init();

  //Set some PID class parameters
  drive_straight_controller.SetMode(1);
  drive_straight_controller.SetOutputLimits(-100, 100);
  drive_straight_controller.SetSampleTime(10);

  //Begin serial comms
  Serial.begin(9600);
}

void loop()
{
    //Timer flipping logic
  if(millis() - prev_time >= state_flip_period)
  {
    int buff = -1 * control_state.getLinearState(); // Logic to flip sign of current linear state (change direction) 
    control_state.setLinearState(buff);
    control_state.setRotationState(control_state.getRotationState()*-1);
    //Reset timer variables
    prev_time = millis();
  }
  
  //update sodar
//  mysodar.update();

  //update accelerometer
  accel.update();
  
  //PID input from accelerometer
  input = accel.get_ang_vel('z');
  
  //update PID setpoint
  setpoint = control_state.getRotationState();
  
  //Compute PID drive_straight_controller
  drive_straight_controller.Compute();
  
  //Set motor power based on feedback from drive_straight_controller PID class and linear state
  raw_motor_control(control_state.getLinearState()-output, control_state.getLinearState()+output);
  

  //Print
  Serial.print("input: "); Serial.print(input); Serial.print(" setpoint: "); Serial.print(setpoint);
  Serial.print(" output: "); Serial.println(output);
}
