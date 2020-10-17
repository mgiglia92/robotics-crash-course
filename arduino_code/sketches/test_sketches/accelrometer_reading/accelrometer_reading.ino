#include <MPU6050.h>
#include <HC_SR04.h>
#define SDA 4
#define SCL 5

MPU6050 sensor(SDA, SCL);
HC_SR04 dist(10, 2, 0);
int distance;

void setup() {
Serial.begin(9600);
Serial.print("start");
sensor.initialize();
sensor.update();
//pinMode(A0, INPUT);
dist.begin();
dist.start();
}

void loop() {
  if(dist.isFinished())
  {
    distance = dist.getRange();
    dist.start();
  }
  sensor.update();
Serial.print("accel: "); Serial.print(sensor.get_accel('x')); Serial.print(" dist: "); Serial.print(distance);
Serial.print(" | logic lvl: "); Serial.println(analogRead(A0));
}
