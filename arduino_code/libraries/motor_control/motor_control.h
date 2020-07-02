// #include "motor_control.h"

//  Sets up motors - sets pins
void motor_setup();

// Conversion utilities
int convert_pwr_to_pwm(int pwr);

// ------  Motor Control Functions ------

// pwr should always be 0-100
// commands: 0 - Forward
//           1 - Backward
//           2 - Right
//           3 - Left
void control(int command, int pwr);

// Cuts power to all motors
void dwell();

// Moves foward
void backward(int pwm);

// Moves backward
void forward(int pwm);

// Rotates left using differential drive style
void diff_left(int pwm);

// Rotates right using differential drive style
void diff_right(int pwm);

//control motors directly
void raw_motor_control(int pwm1, int pwm2);
