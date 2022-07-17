/*
 * 2-stack-overflow-sol.ino -- stack overflow solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

// NOTE: this solution was compiled for the Arduino Uno Rev3 using
// version 1.8.5 of the official Arduino AVR core and version 1.18.9 of
// the Arduino IDE

unsigned int fib(unsigned int n)
{
  Serial.print("a:");
  Serial.print((uintptr_t) &n);
  Serial.print(": ");
  Serial.println(n);

  if (n <= 1) return n;

  return fib(n - 1) + fib(n - 2);
}

void setup()
{
  Serial.begin(9600);

  fib(10000);
}

void loop()
{
}
