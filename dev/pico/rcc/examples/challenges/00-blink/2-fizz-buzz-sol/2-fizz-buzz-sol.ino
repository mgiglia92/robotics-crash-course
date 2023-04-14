// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * 2-fizz-buzz-sol.ino -- FizzBuzz solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#define BLINK_DELAY_MS 200
#define CNT_DELAY_MS   400


void blink_cnt(int cnt)
{
  for (int i = 0; i < cnt; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(BLINK_DELAY_MS);
    digitalWrite(LED_BUILTIN, LOW);
    delay(BLINK_DELAY_MS);
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  // ensure LED_BUILTIN is off before we begin
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);

  for (int i = 1; i <= 100; i++) {
    if (!(i % 15)) {
      Serial.println("FizzBuzz");
      blink_cnt(3);
    } else if (!(i % 5)) {
      Serial.println("Buzz");
      blink_cnt(2);
    } else if (!(i % 3)) {
      Serial.println("Fizz");
      blink_cnt(1);
    } else {
      Serial.println(i);
    }

    delay(CNT_DELAY_MS);
  }
}

void loop()
{
  // since we're only running our code once, we can disregard loop()
}
