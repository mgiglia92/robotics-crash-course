/*
 * 1-conversions-sol.ino -- unit conversion functions solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#define SOUND_METERS_PER_MICROSECOND      0.000346
#define SOUND_CENTIMETERS_PER_MICROSECOND 0.0346

#define DUMMY_DURATION_US 5000


float duration2meters(unsigned long duration_us)
{
  return SOUND_METERS_PER_MICROSECOND * (duration_us / 2);
}

float duration2centimeters(unsigned long duration_us)
{
  return SOUND_CENTIMETERS_PER_MICROSECOND * (duration_us / 2);
}

void setup()
{
  Serial.begin(9600);

  Serial.print("A ");
  Serial.print(DUMMY_DURATION_US);
  Serial.print(" microsecond duration means an object is ");
  Serial.print(duration2meters(DUMMY_DURATION_US));
  Serial.print(" meters or ");
  Serial.print(duration2centimeters(DUMMY_DURATION_US));
  Serial.println(" centimeter away");
}

void loop()
{
}
