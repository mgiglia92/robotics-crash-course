// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * 0-random-longs-sol.ino -- generate random longs solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

long randoms[16];


void setup()
{
  Serial.begin(9600);

  for (int i = 0; i < 16; i++) {
    randoms[i] = random(2147483648);  // aka LONG_MAX in <limits.h>
    Serial.println(randoms[i]);
  }
}

void loop()
{
}
