#include <Timer.h>
#include <HC_SR04.h>
#include <MPU6050.h>

#define TRIG_PIN 10
#define ECHO_PIN 2
#define ECHO_INT 0

HC_SR04 dist_sensor(TRIG_PIN, ECHO_PIN, ECHO_INT);
MPU6050 accel(4, 5);
Timer t1(1000);

int distance = 9999;

void setup() {
  pinMode(13, OUTPUT); //Let pin 13 control LED
  Serial.begin(9600);

  //Initialize distance sensor and start pinging for distances
  dist_sensor.begin();
  dist_sensor.start();
}

void loop() {
  if(dist_sensor.isFinished())
    {
      distance = dist_sensor.getRange();
      dist_sensor.start();
    } 
    
   if(distance < 10) { t1.start();}
   
   if(!t1.is_finished(false))
   {
    digitalWrite(13, HIGH); //Turn LED on
    Serial.print("do the thing");
   }
   
   else
   {
    digitalWrite(13, LOW); //Turn LED off
    Serial.print("Dont do the thing");
   }
   Serial.print("distance: "); Serial.println(distance);
}
