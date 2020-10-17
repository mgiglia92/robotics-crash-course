//Last code had a bug with accel updateing fixed here

 

//Turn Servo to test it it should snap back; to test motors type number (0-255) in Serial Monitor

 


#include <Servo.h>
#include <motor_control.h>
#include <HC_SR04.h>
#include <MPU6050.h>
#define ECHO_PIN 2
#define TRIG_PIN 10
#define ECHO_INT 0
#define SDA 4
#define SCL 5

 

MPU6050 sensor(SDA, SCL);

 

HC_SR04 distance_sensor(TRIG_PIN, ECHO_PIN, ECHO_INT);
int yB;
String inString = "";

 

Servo servo1;

 

void setup() {
  // put your setup code here, to run once:
  servo1.attach(9);
  servo1.write(90);
//Turn Servo to test it
  
  Serial.begin(9600);

 

  distance_sensor.begin();
  distance_sensor.start();
  sensor.initialize();
  sensor.update();

 

}

 

void loop() {
  sensor.update();
  Serial.print("Accel: X");
  Serial.print(sensor.get_accel('x'));
  Serial.print(", Y");
  Serial.print(sensor.get_accel('y'));
  Serial.print(", Z");
  Serial.println(sensor.get_accel('z'));
  
  if (distance_sensor.isFinished()) {
    Serial.print("Distance: ");
    Serial.println(distance_sensor.getRange());
    distance_sensor.start();
  }
  
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;

 

    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      yB = inString.toInt();
      inString = "";
          }
  }
  forward(yB);
  delay(100);
}
