/*
 * Test to see if sensor is working, connect sensor and run, should get 
 * reasonable values in serial monitor
 */

#define TRIG_PIN 10
#define ECHO_PIN 2
#define ECHO_INT 0

#include <HC_SR04.h>

HC_SR04 sensor(TRIG_PIN, ECHO_PIN, ECHO_INT);

void setup()
{
  //Initialize sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  // HC_SR04 initialize procedure
  sensor.begin();
  sensor.start();

  //Begin serial comms
  Serial.begin(9600);
}

void loop()
{
  if(sensor.isFinished())
  {
    Serial.print("distance: "); Serial.println(sensor.getRange());
    //Restart sensor detection
    sensor.start();
  }

}
