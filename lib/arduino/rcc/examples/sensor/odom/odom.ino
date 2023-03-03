// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * odom.ino -- demonstrate the functionality of the encoders
 *
 * Copyright (C) 2020-2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <rcc.h>


#define TICKS_FOR_90_DEGREES 12


Default_Odom odom;


void turn_90_deg_left(void)
{
  // get the initial encoder count
  unsigned long count_start = odom.getRightCount();

  // NOTE:
  // if instead we wanted to get the left encoder count we'd call
  // odom.getLeftCount() as opposed to odom.getRightCount()

  unsigned long count_current;

  Serial.print("count start: ");
  Serial.println(count_start);

  // keep on turning the right wheel as long as the difference between
  // the current encoder count and the initial encoder count do not
  // exceed the defined limit
  while ((count_current = odom.getRightCount()) - count_start <= TICKS_FOR_90_DEGREES) {
    Serial.print("current count: ");
    Serial.println(count_current);
    rawMotorCtrl(0, 192);
  }

  rawMotorCtrl(0, 0);
}

void setup()
{
  Serial.begin(9600);

  turn_90_deg_left();
}

void loop()
{
}
