// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * non-blocking.ino -- demonstrate the functionality of the ultrasonic
 *                     sensor using the non-blocking interface
 *
 * Copyright (C) 2020  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <Servo.h>

#include <rcc.h>


#define DESIRED_DISTANCE_CM 30
#define SERVO_CENTER_DEG    90


Servo actuator;
HC_SR04_async ultrasonic;

unsigned long duration_us;


void setup()
{
  // attach servo to servo pin, and rotate to point forward
  actuator.attach(RCC_SERVO_PIN);
  actuator.write(SERVO_CENTER_DEG);

  // initialize the HC-SR04
  ultrasonic.begin();

  // send the initial pulse
  ultrasonic.pulse();
}

void loop()
{
  // get the reading when ready
  if (ultrasonic.isDone()) {
    duration_us = ultrasonic.getDuration();

    // send the next pulse to read
    ultrasonic.pulse();
  }

  // move forward until we're 30cm away from something
  if (duration2centimeters(duration_us) > DESIRED_DISTANCE_CM) {
    rawMotorCtrl(200, 200);
  } else {
    rawMotorCtrl(0, 0);
  }
}
