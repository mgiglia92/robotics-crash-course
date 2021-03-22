//Include necessary libraries
#include <motor_control.h>
#include <MPU6050.h>
#include <PID_v1.h>

//Define variables for the controller class
double input, output;
double setpoint=90;
int base_power = 155;

//Controller class instantiatoin
double kp=2, ki=0, kd=0;
PID controller(&input, &output, &setpoint, kp, ki, kd, DIRECT);

//Initialize IMU
MPU6050 IMU(4,5);

void setup() {
  motor_setup(); //Setup motor pins
  IMU.initialize(); //Initialize the MPU6050 sensor (IMU)
  delay(500); //Add a delay to prevent calibration issues
  IMU.calibrate(); //Calibrate the IMU (note: don't let robot move during this step)

  //Set controller hyper parameters
  controller.SetMode(1);
  controller.SetOutputLimits(-100, 100);
  controller.SetSampleTime(50); //50ms between samples

  //Start serial communication
  Serial.begin(9600);
}

void loop() {
  //Update the IMU sensor
  IMU.update();
  //Update the input variable for the controller (this is omega_actual)
  input = IMU.get_ang_vel('z');
  //Compute the PID controller output
  controller.Compute();
  //Set motor power based off of base power and pid controller output
  raw_motor_control(base_power + output, base_power - output);
  //Print stuff for debugging/analysis
//  Serial.print("output: "); Serial.print(output); Serial.print(" | input: "); Serial.println(input);
  Serial.print(output); Serial.print(','); Serial.println(input);

}
