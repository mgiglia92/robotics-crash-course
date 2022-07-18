/*
 * 0-non-primitive-sol.ino -- user-defined types solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <rcc.h>


typedef struct timing_event_s {
  unsigned long prv;
  unsigned long dt;
  bool enabled;
} timing_event_t;


timing_event_t events[4];
size_t         enabled_cnt;


void setup()
{
  Serial.begin(9600);

  for (size_t i = 0; i < ARRAYSIZE(events); i++) {
    events[i].dt  = random(2000);
    events[i].prv = millis();
  }

  // since everything is bounded by our first timing event, we want to
  // ensure that it has the largest dt so that all of the other events
  // can run at least two times before they are once again disabled
  events[0].dt += 4000;

  events[0].enabled = true;
  enabled_cnt       = 1;
}

void loop()
{
  unsigned long cur_ms;

  for (size_t i = 0; i < ARRAYSIZE(events); i++) {
    if (events[i].enabled && ((cur_ms = millis()) - events[i].prv >= events[i].dt)) {
      unsigned long dt = cur_ms - events[i].prv;
      events[i].prv = cur_ms;

      Serial.print("event: ");
      Serial.print(i);
      Serial.print(" dt: ");
      Serial.println(dt);

      // only enable other events if we're in the first timing event
      if (!i) {
        // check if we've enabled all of our events and reset the
        // initial state if that is the case
        if (enabled_cnt >= ARRAYSIZE(events)) {
          for (size_t i = 1; i < ARRAYSIZE(events); i++)
            events[i].enabled = false;

          // reset our enabled count/index
          enabled_cnt = 1;

          // since we've disabled all other events, we should exit the
          // current itteration of the for loop as we'd re-enabled the
          // second event immediately
          continue;
        }

        // enable the next timing event
        events[enabled_cnt++].enabled = true;
      }
    }
  }
}
