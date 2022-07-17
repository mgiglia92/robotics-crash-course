/*
 * 0-timing.ino -- using timers
 *
 * Using millis(), blink the built-in LED and print a message over UART
 * at variable rates.  At first blink the LED every millisecond, but
 * after each successful cycle, increase the duration by a power of two,
 * up to 8192ms.  For the message that should be sent over UART, send it
 * every second for 10 seconds, after which it should be sent every two
 * seconds for 10 seconds.  These patterns should repeat indefinitely.
 */

// define any constants you may need up here

// declare any timing and logic variables you may need up here, remember
// that the datatype returned by millis() is an unsigned long

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  // initialize any timing variables you may need here
}

void loop()
{
  if (/* blink timing condition */) {
  }

  if (/* UART timing condition */) {
  }
}
