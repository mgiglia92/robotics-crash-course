// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * 0-sos-sol.ino -- SOS solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#define SHORT_MS 400
#define LONG_MS  800
#define PAUSE_MS 600


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // S
  digitalWrite(LED_BUILTIN, HIGH);
  delay(SHORT_MS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(SHORT_MS);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(SHORT_MS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(SHORT_MS);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(SHORT_MS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(SHORT_MS);

  // O
  digitalWrite(LED_BUILTIN, HIGH);
  delay(LONG_MS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(LONG_MS);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(LONG_MS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(LONG_MS);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(LONG_MS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(LONG_MS);

  // S
  digitalWrite(LED_BUILTIN, HIGH);
  delay(SHORT_MS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(SHORT_MS);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(SHORT_MS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(SHORT_MS);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(SHORT_MS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(SHORT_MS);

  // (optional) visible pause
  delay(PAUSE_MS);
}
