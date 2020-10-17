// This will demonstrate the motor_control library.
// There is code below for each type of driving - uncomment a line and then upload.

#include <motor_control.h>;

void setup() {
  // put your setup code here, to run once:
  motor_setup();
}

void loop() {
  // Drives forward
  // forward(100);

  // Drives backwards
  // backward(100);

  // Turns right (differential)
  // diff_right(100);

  // Turns left (differential)
  // diff_left(100);

  // Turns right (moving forward)
  // raw_motor_control(150, 100);

  // Turns left (moving forward)
  // raw_motor_control(100, 150);
}
