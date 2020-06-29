// #include "motor_control.h"

//motor control setup
void motor_setup();

//Conversion utilities
int convert_pwr_to_pwm(int pwr);

//Motor Control Functions
// General control function, can pass a string and power to choose type of control defined below
// pwr should always be 0-100
void control(int command, int pwr);

//Will cut power to all motors
void dwell();

//Moves foward 
void backward(int pwm);

//Moves backward
void forward(int pwm);

//Rotate left using differential drive style
void diff_left(int pwm);

//Rotate right using differential drive style
void diff_right(int pwm);

//control motors directly
void raw_motor_control(int pwm1, int pwm2);