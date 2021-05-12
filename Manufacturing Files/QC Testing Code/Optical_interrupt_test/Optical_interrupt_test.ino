/*
 * Test to see if optical interrupt is working.
 * Optical Interrupt: OUT, VCC, GND -> D2, +5V, GND
 * Pass something through sensor and see that value increases
 */

volatile int count=0;

void setup()
{
  pinMode(2, INPUT);
attachInterrupt(digitalPinToInterrupt(2), increment, RISING);

  //Start serial comms
  Serial.begin(9600);
}

void loop()
{
  //Print value to screen
  Serial.print("count: "); Serial.println(count);
}

void increment()
{
  count++;
}
