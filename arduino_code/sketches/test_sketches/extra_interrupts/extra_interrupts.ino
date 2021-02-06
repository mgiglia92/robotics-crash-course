#include "PinChangeInterrupt.h"
#define RIGHT_ENCODER 10
#define LEFT_ENCODER 9
int right_count = 0;
int left_count = 0;

void setup(){
  Serial.begin(9600);
  attachPCINT(digitalPinToPCINT(RIGHT_ENCODER), do_right_count, RISING);
  attachPCINT(digitalPinToPCINT(LEFT_ENCODER), do_left_count, RISING);
  
}


void loop(){
  Serial.print("right_count: "); Serial.print(right_count); Serial.print(" | left_count: "); Serial.println(left_count);
}

void do_right_count(){
  right_count++;
}
 void do_left_count(){
  left_count++;
 }
