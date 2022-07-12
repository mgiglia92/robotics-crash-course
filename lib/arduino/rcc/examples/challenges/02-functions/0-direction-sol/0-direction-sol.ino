// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * 0-direction-sol.ino -- direction functions solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <rcc.h>


#define MOVEMENT_DELAY_MS 2000


void forward(int pwm)
{
  rawMotorCtrl(pwm, pwm);
}

void backward(int pwm)
{
  rawMotorCtrl(-pwm, -pwm);
}

void left(int pwm)
{
  rawMotorCtrl(0, pwm);
}

void right(int pwm)
{
  rawMotorCtrl(pwm, 0);
}

void setup()
{
  // initialize motor hardware
  motorSetup();

  // initialize UART hardware for printing current function
  Serial.begin(9600);
}

void loop()
{
  Serial.println("going forward!");
  forward(200);
  delay(MOVEMENT_DELAY_MS);

  Serial.println("going backward!");
  backward(200);
  delay(MOVEMENT_DELAY_MS);

  Serial.println("going left!");
  left(200);
  delay(MOVEMENT_DELAY_MS);

  Serial.println("going right!");
  right(200);
  delay(MOVEMENT_DELAY_MS);
}
