#include <MPU6050.h>


#define SDA 4
#define SCL 5

MPU6050 sensor(SDA, SCL);

void setup() {
sensor.initialize();
sensor.update();
Serial.begin(9600);
}

void loop() {
  sensor.update();
Serial.println(sensor.get_accel('x'));
}
