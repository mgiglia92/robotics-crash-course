/*
 * 2-stack-overflow-sol.ino -- stack overflow solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

unsigned int fib(unsigned int n)
{
  Serial.print("0x");
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
