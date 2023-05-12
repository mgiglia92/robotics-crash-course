// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * motor.ino -- demonstrate the functionality of the motors
 *
 * Copyright (C) 2022  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <rcc.h>


void setup()
{
  // initialize the motor controller
  motorSetup();

  Serial.begin(9600);
}

void loop()
{
  Serial.println("moving forwards for 1s");
  rawMotorCtrl(200, 0);
  delay(1000);
  Serial.println("moving backwards for 1s");
  rawMotorCtrl(-200, 0);
  delay(1000);
}
