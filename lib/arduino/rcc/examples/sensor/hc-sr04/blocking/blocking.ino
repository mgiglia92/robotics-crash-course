// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * blocking.ino -- demonstrate the functionality of the ultrasonic
 *                 sensor using the blocking interface
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <Servo.h>

#include <rcc.h>


#define DESIRED_DISTANCE_CM 30
#define SERVO_CENTER_DEG    90


Servo actuator;
HC_SR04 ultrasonic;

unsigned long duration_us;


void setup()
{
  // attach servo to servo pin, and rotate to point forward
  actuator.attach(RCC_SERVO_PIN);
  actuator.write(SERVO_CENTER_DEG);

  // initialize the HC-SR04
  ultrasonic.begin();
}

void loop()
{
  // get the reading
  duration_us = ultrasonic.pulse();

  // move forward until we're 30cm away from something
  if (duration2centimeters(duration_us) > DESIRED_DISTANCE_CM) {
    rawMotorCtrl(200, 200);
  } else {
    rawMotorCtrl(0, 0);
  }
}
