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

  // send the initial pulse to read with the default timeout of 5000
  // microseconds (defined as RCC_HC_SR04_TIMEOUT_US) we can also change
  // the timeout by specifying a timeout when calling the pulse() method
  ultrasonic.pulse();
  // ultrasonic.pulse(RCC_HC_SR04_TIMEOUT_US);
  // ultrasonic.pulse(5000);

  Serial.begin(9600);
}

void loop()
{
  // get the reading when ready
  if (ultrasonic.isDone()) {
    duration_us = ultrasonic.getDuration();

    // send the next pulse to read with the default timeout
    ultrasonic.pulse();
    // ultrasonic.pulse(RCC_HC_SR04_TIMEOUT_US);
    // ultrasonic.pulse(5000);
  }

  // move forward until we're 30cm away from something
  // NOTE: if the returned pulse duration is zero, we know that pulse()
  // timed out which means an object is sufficiently far away that we
  // don't need to worry about it
  if ((duration2centimeters(duration_us) > DESIRED_DISTANCE_CM) || !duration_us) {
    rawMotorCtrl(200, 200);
  } else {
    rawMotorCtrl(0, 0);
  }

  Serial.print("distance cm: ");
  Serial.println(duration2centimeters(duration_us));
}
