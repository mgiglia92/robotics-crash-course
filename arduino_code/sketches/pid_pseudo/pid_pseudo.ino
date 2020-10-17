#include <PID_v1.h>


double input;
double output;
double setpoint;
double kp=10;
double ki;
double kd=5;
PID controller(&input, &output, &setpoint, kp, ki, kd, DIRECT);

//instantiate distance sensor

void setup() {
  // put your setup code here, to run once:

//Set Controller Sample Time
controller.setSampleTime(25);
//Set Controller Output Limits
controller.setOutputLimits(-100,100);
//Set controller mode to ON
controller.setMode(1);`

}

void loop() {
  // put your main code here, to run repeatedly:

//Update input (typically from a sensor hint hint)(sensor.isFinished())

//run controller.Compute();
controller.Compute();

//Update power to motors
raw_motor_control(output, output);

}
