/* This example shows how to use continuous mode to take
  range measurements with the VL53L0X. It is based on
  vl53l0x_ContinuousRanging_Example.c from the VL53L0X API.

  The range readings are in units of mm. */

#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

unsigned long cur, prev, t0, t1 = 0;

void setup()
{
  Serial.begin(9600);
  initLidar();
}

void loop()
{
  t0 = millis();
  uint16_t dist_mm = getFastReading();
  t1 = millis();
  Serial.print(dist_mm);
  Serial.print(" | ");
  Serial.print(t1 - t0);
  Serial.println();
}

void initLidar()
{
  Wire.begin();

  sensor.setTimeout(200);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous(0);

}

uint16_t getFastReading()
{

  uint16_t range = sensor.readReg16Bit(sensor.RESULT_RANGE_STATUS + 10);
  //  sensor.writeReg(sensor.SYSTEM_INTERRUPT_CLEAR, 0x01);
  return range;
}
