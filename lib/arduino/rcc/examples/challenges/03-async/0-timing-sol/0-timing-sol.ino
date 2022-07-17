/*
 * 0-timing-sol.ino -- using timers solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#define MAX_BLINK_MS        8192
#define MAX_UART_MSG_SUM_MS 10000


unsigned long cur_ms;

unsigned long blink_prv_ms;
unsigned long uart_prv_ms;

unsigned long blink_dt_ms;
unsigned long uart_dt_ms;

uint8_t       led_state;
unsigned long uart_msg_sum_ms;


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  // we want to start blinking our LED by setting it HIGH
  led_state = HIGH;

  // initialize our time deltas appropriately
  blink_dt_ms = 1;
  uart_dt_ms  = 1000;

  // initialize previous time events to the initialization time
  blink_prv_ms = uart_prv_ms = millis();
}

void loop()
{
  if ((cur_ms = millis()) - blink_prv_ms >= blink_dt_ms) {
    blink_prv_ms = cur_ms;

    digitalWrite(LED_BUILTIN, led_state);

    // double our blink cycle time at the end of cycle
    // (when the LED is LOW)
    if (!led_state) blink_dt_ms *= 2;

    // reset our cycle time once we exceed the max
    if (blink_dt_ms > MAX_BLINK_MS) blink_dt_ms = 1;

    led_state = !led_state;
  }

  if ((cur_ms = millis()) - uart_prv_ms >= uart_dt_ms) {
    uart_msg_sum_ms += cur_ms - uart_prv_ms;
    uart_prv_ms      = cur_ms;

    Serial.println(uart_dt_ms);

    // if we've sent the same message for over 10s,
    // it's time to change our delta value
    if (uart_msg_sum_ms >= MAX_UART_MSG_SUM_MS) {
      uart_msg_sum_ms = 0;
      uart_dt_ms      = (uart_dt_ms == 1000) ? 2000 : 1000;
    }
  }
}
