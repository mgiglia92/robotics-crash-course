#include <MPU6050.h>
#include <motor_control.h>
#include <PID_v1.h>

//Define Variables we'll be using for the controller class
double input, output;//Input will be the sensor reading into the controller (our actual state)
                      //Output will be the command the PID controller wants you to use to get zero error
double setpoint=0; //The desired state the controller is trying to get to
int power = 155;//base power to the robot (think of this as the amount of power to move foward/backward

//Controller Class instantiation
double kp=2, ki=0, kd=0; //Controller Gains
PID straight(&input, &output, &setpoint, kp, ki, kd, DIRECT);

//IMU initailize
MPU6050 IMU(4, 5);

void setup()
{
  motor_setup();//setup motor pins
  IMU.initialize();//initialize MPU6050 sensor
  delay(500); //Add delay so finger press on reset button does not mess up calibration
  IMU.calibrate();  //calibrate the sensor(robot cannot move during this time)

  //Set controller Hyper parameters
  straight.SetMode(1);
  straight.SetOutputLimits(-100, 100);
  straight.SetSampleTime(50); //50ms between samples

  //Start serial communication
  Serial.begin(9600);
}

void loop()
{
  //Update sensor
  IMU.update();
  //Update input value to controller by requesting 'z' angular velocity from IMU
  input = IMU.get_ang_vel('z');
  //compute PID controller output
  straight.Compute();
  //Set motors to base power +/- PID output
  raw_motor_control(power-output, power+output); 

  //Print for debugging/analysis
  Serial.print("output: "); Serial.print(output); Serial.print(" input: "); Serial.println(input);
  
}
