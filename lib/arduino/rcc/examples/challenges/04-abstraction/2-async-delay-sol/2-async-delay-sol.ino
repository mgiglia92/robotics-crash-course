/*
 * 2-async-delay-sol.ino -- async delay class solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#define BLINK_DELAY_US 500000
#define UART_DELAY_MS  2000


class async_delay {
  unsigned long (*timer)(void);

  unsigned long cur_time = 0;
  unsigned long prv_time = 0;

  unsigned long prv_duration = 0;

public:
  async_delay(unsigned long (*timer)(void));
  bool delay(unsigned long duration = 0);
};


async_delay::async_delay(unsigned long (*timer)(void))
{
  this->timer = timer;
}

bool async_delay::delay(unsigned long duration = 0)
{
  // zero duration is meaningless!
  if (!duration) return true;

  // deal with changing the duration
  if (prv_duration != duration) {
    prv_duration = duration;
    prv_time     = timer();

    return false;
  }

  if ((cur_time = timer()) - prv_time >= duration) {
    prv_time = cur_time;

    return true;
  }

  return false;
}


async_delay blink(micros);
async_delay uart(millis);


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  if (blink.delay(BLINK_DELAY_US))
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  if (uart.delay(UART_DELAY_MS))
    Serial.println("Sending some asynchronous swag B)");
}
