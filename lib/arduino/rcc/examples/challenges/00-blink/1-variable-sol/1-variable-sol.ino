// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * 1-variable-sol.ino -- variable length blinking solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  for (int i = 10; i <= 1000; i += 10) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(i);
    digitalWrite(LED_BUILTIN, LOW);
    delay(i);
  }
}
