//Include relevant libraries
#include <MPU6050.h>
#include <HC_SR04.h>
#include "PinChangeInterrupt.h"

//Pin definitions as well as some robot constants
#define TRIG_PIN 10
#define ECHO_PIN 2
#define ECHO_INT 0
#define RIGHT_ENCODER 13 //pin for encoders
#define LEFT_ENCODER 4
#define PPR 20 //# of pulses per revolution of the wheel (# slots on the encoder wheel)
#define WHEEL_DIAMETER 0.08 //meters
#define WHEEL_BASE 0.13208 //meters

//Counting variables for the left and the right encoders (voltaile is necessary, because
// we are using what is called an interrupt)
volatile int right_count = 0; 
volatile int left_count = 0;

//Motor power variables
int pwr_right = 0;
int pwr_left = 0;

// Instantiate sensors
MPU6050 IMU(4,5);
HC_SR04 dist(TRIG_PIN, ECHO_PIN, ECHO_INT);

void setup() {
  attachPCINT(digitalPinToPCINT(RIGHT_ENCODER), do_right_count, RISING);
  attachPCINT(digitalPinToPCINT(LEFT_ENCODER), do_left_count, RISING);
  
  IMU.initialize();
  IMU.calibrate();
  dist.begin();
  Serial.begin(115200);
}

void loop() {
 //Use the loop to do the tasks defined in the assignment document.

 //Print some stuff to the serial monitor
 Serial.print("left count: "); Serial.print(left_count);
 Serial.print(" | right count: "); Serial.println(right_count);
}

//---------------------------------------------------------------------------------------------------------------
//Interrupt Functions
void do_right_count(){
  if(pwr_right >= 0){ right_count++;}
  else if(pwr_right < 0) {right_count--;}
}
 void do_left_count(){
  if(pwr_left >= 0){ left_count++;}
  else if(pwr_left < 0) {left_count--;}
 }
