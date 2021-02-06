#include "motor_control.h"
#include "Arduino.h"

//Motor control setup
//Motor control setup
void motor_setup()
{
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}

//Conversion utilities
int convert_pwr_to_pwm(int pwr)
{
  //Scale power (which should be 0-100, to 0-255 for 8bit pwm signal value)
  return map(pwr, 0, 100, 0, 255);
}

//Motor Control Functions

//Will cut power to all motors
// void dwell()
// {
//   //Cut power to motors
//   analogWrite(speedPinL,0);
//   analogWrite(speedPinR,0);
// }

// //Moves backward
// void backward(int pwm) 
// {
//   // Set direction pins
//   digitalWrite(RightDirectPin1, LOW);
//   digitalWrite(RightDirectPin2,HIGH);
//   digitalWrite(LeftDirectPin1,LOW);
//   digitalWrite(LeftDirectPin2,HIGH);
//   //Write to speed pins
//   analogWrite(speedPinL,pwm);
//   analogWrite(speedPinR,pwm);
// }

// //Moves forward
// void forward(int pwm)
// {
//   // Set direction pins
//   digitalWrite(RightDirectPin1, HIGH);
//   digitalWrite(RightDirectPin2,LOW);
//   digitalWrite(LeftDirectPin1,HIGH);
//   digitalWrite(LeftDirectPin2,LOW);


//   //Write motor power to speed pins
//   analogWrite(speedPinL,pwm);
//   analogWrite(speedPinR,pwm);
// }

// //Rotate left using differential drive style
// void diff_left(int pwm)
// {
//   // Set direction pins
//   digitalWrite(RightDirectPin1, HIGH);
//   digitalWrite(RightDirectPin2,LOW);
//   digitalWrite(LeftDirectPin1,LOW);
//   digitalWrite(LeftDirectPin2,HIGH);
//   //Write to speed pins
//   analogWrite(speedPinL,pwm);
//   analogWrite(speedPinR,pwm);
// }

// //Rotate right using differential drive style
// void diff_right(int pwm)
// {
//   // Set direction pins
//   digitalWrite(RightDirectPin1, LOW);
//   digitalWrite(RightDirectPin2,HIGH);
//   digitalWrite(LeftDirectPin1,HIGH);
//   digitalWrite(LeftDirectPin2,LOW);
//   // Write to speed pins
//   analogWrite(speedPinL,pwm);
//   analogWrite(speedPinR,pwm);
// }

// // General control function, can pass a command integer and power to choose type of control defined below
// // pwr should always be 0-100
// void control(int command, int pwr)
// {
//   //Convert pwr to pwm to pass to raw motor control functions
//   int pwm = convert_pwr_to_pwm(pwr);

//   //Search for which command
//   //0 - forward
//   //1 - backward
//   //2 - right
//   //3 - left 
//   if(command == 0){forward(pwm);}
//   if(command == 1){backward(pwm);}
//   if(command == 2){diff_right(pwm);}
//   if(command == 3){diff_left(pwm);}
//   if(command == -1){dwell();}
// }

//Control motors with raw pwm values (-255 -> 255)
//This function will constrain the values to 0-255 in case it is sent bad numbers
//Will change direction pins appropriately based on +/- value of pwm arguments
void raw_motor_control(int left, int right)
{
  // Set direction pins dependent on if pwm is negative or positive
 
  if(left >= 0){digitalWrite(7,HIGH); digitalWrite(8,LOW);}
  else if(left < 0){digitalWrite(7,LOW); digitalWrite(8,HIGH);}

  if(right >= 0) {digitalWrite(11, HIGH);digitalWrite(12,LOW); }
  else if(right < 0){ digitalWrite(11, LOW);digitalWrite(12,HIGH); }
  
  //constrain input to 0-255, take absolute value of power first before constraining
  left = constrain(abs(left), 0, 255);
  right = constrain(abs(right), 0, 255);
  //Write motor power to speed pins
  analogWrite(5,left);
  analogWrite(6,right);
}