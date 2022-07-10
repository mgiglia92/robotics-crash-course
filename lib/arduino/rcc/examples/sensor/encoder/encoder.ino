// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * encoder.ino -- demonstrate the functionality of the encoders
 *
 * Copyright (C) 2020-2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <rcc.h>


#define TICKS_FOR_90_DEGREES 20


void turn_90_deg_left(void)
{
  // get the initial encoder count
  unsigned long count_start = encoderGetRightCount();

  // NOTE:
  // if instead we wanted to get the left encoder count we'd call
  // encoderGetLeftCount() as opposed to encoderGetRightCount()

  // keep on turning the right wheel as long as the difference between
  // the current encoder count and the initial encoder count do not
  // exceed the defined limit
  while (encoderGetRightCount() - count_start < TICKS_FOR_90_DEGREES)
    rawMotorCtrl(0, 192);

  rawMotorCtrl(0, 0);
}

void setup()
{
  encoderSetup();  // initialize encoder hardware

  turn_90_deg_left();
}

void loop()
{
}
