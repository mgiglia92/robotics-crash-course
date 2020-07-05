#include <MPU6050.h>
#include <motor_control.h>
#include <PID_v1.h>

//Define Variables we'll be connecting to
double input, output;
double setpoint=0;
int power = 155;//power to the robot

//Specify the links and initial tuning parameters
double kp=2, ki=0, kd=0;
PID straight(&input, &output, &setpoint, kp, ki, kd, DIRECT);

//IMU initailize
MPU6050 IMU(4, 5);

void setup()
{
  motor_setup();//setup motor pins
  IMU.initialize();//initialize MPU6050 sensor
  delay(500); //Add delay so finger press on reset button does not mess up calibration
  IMU.calibrate();  //calibrate the sensor(robot cannot move during this time)
  straight.SetMode(1);
  straight.SetOutputLimits(-100, 100);
  straight.SetSampleTime(50);
  Serial.begin(9600);
}

void loop()
{
  //Update sensor
  IMU.update();
  //compute
  straight.Compute();
  input = IMU.get_ang_vel('z');
  raw_motor_control(power-output, power+output); //Need to make schema of robot for this
  Serial.print("output: "); Serial.print(output); Serial.print(" input: "); Serial.println(input);
  
}
