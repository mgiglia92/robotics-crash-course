#include "motor_control.h"
#include "Arduino.h"

//Define L298N Dual H-Bridge Motor Controller Pins
#define speedPinR 3   // RIGHT PWM pin connect MODEL-X ENA
#define RightDirectPin1  12    //  Right Motor direction pin 1 to MODEL-X IN1 
#define RightDirectPin2  11    // Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6        //  Left PWM pin connect MODEL-X ENB
#define LeftDirectPin1  7    // Left Motor direction pin 1 to MODEL-X IN3
#define LeftDirectPin2  8   ///Left Motor direction pin 1 to MODEL-X IN4

//Motor control setup
void motor_setup()
{
  pinMode(RightDirectPin1, OUTPUT);
  pinMode(RightDirectPin2, OUTPUT);
  pinMode(speedPinL, OUTPUT);
  pinMode(LeftDirectPin1, OUTPUT);
  pinMode(LeftDirectPin2, OUTPUT);
  pinMode(speedPinR, OUTPUT);
}

//Conversion utilities
int convert_pwr_to_pwm(int pwr)
{
  //Scale power (which should be 0-100, to 0-255 for 8bit pwm signal value)
  return map(pwr, 0, 100, 0, 255);
}

//Motor Control Functions

//Will cut power to all motors
void dwell()
{
  //Cut power to motors
  analogWrite(speedPinL,0);
  analogWrite(speedPinR,0);
}

//Moves backward
void backward(int pwm) 
{
  // Set direction pins
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,HIGH);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,HIGH);
  //Write to speed pins
  analogWrite(speedPinL,pwm);
  analogWrite(speedPinR,pwm);
}

//Moves forward
void forward(int pwm)
{
  // Set direction pins
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,HIGH);
  digitalWrite(LeftDirectPin2,LOW);


  //Write motor power to speed pins
  analogWrite(speedPinL,pwm);
  analogWrite(speedPinR,pwm);
}

//Rotate left using differential drive style
void diff_left(int pwm)
{
  // Set direction pins
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,HIGH);
  //Write to speed pins
  analogWrite(speedPinL,pwm);
  analogWrite(speedPinR,pwm);
}

//Rotate right using differential drive style
void diff_right(int pwm)
{
  // Set direction pins
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,HIGH);
  digitalWrite(LeftDirectPin1,HIGH);
  digitalWrite(LeftDirectPin2,LOW);
  // Write to speed pins
  analogWrite(speedPinL,pwm);
  analogWrite(speedPinR,pwm);
}

// General control function, can pass a command integer and power to choose type of control defined below
// pwr should always be 0-100
void control(int command, int pwr)
{
  //Convert pwr to pwm to pass to raw motor control functions
  int pwm = convert_pwr_to_pwm(pwr);

  //Search for which command
  //0 - forward
  //1 - backward
  //2 - right
  //3 - left 
  if(command == 0){forward(pwm);}
  if(command == 1){backward(pwm);}
  if(command == 2){diff_right(pwm);}
  if(command == 3){diff_left(pwm);}
  if(command == -1){dwell();}
}

//Control motors with raw pwm values (-255 -> 255)
//This function will constrain the values to 0-255 in case it is sent bad numbers
//Will change direction pins appropriately based on +/- value of pwm arguments
void raw_motor_control(int pwmL, int pwmR)
{
  // Set direction pins dependent on if pwm is negative or positive
  if(pwmR >= 0) {digitalWrite(RightDirectPin1, HIGH);digitalWrite(RightDirectPin2,LOW); }
  else if (pwmR < 0){ digitalWrite(RightDirectPin1, LOW);digitalWrite(RightDirectPin2,HIGH); }
  
  if(pwmL >= 0){digitalWrite(LeftDirectPin1,HIGH); digitalWrite(LeftDirectPin2,LOW);}
  else if(pwmL < 0){digitalWrite(LeftDirectPin1,LOW); digitalWrite(LeftDirectPin2,HIGH);}


  //constrain input to 0-255, take absolute value of power first before constraining
  pwmL = constrain(abs(pwmL), 0, 255);
  pwmR = constrain(abs(pwmR), 0, 255);
  //Write motor power to speed pins
  analogWrite(speedPinL,pwmL);
  analogWrite(speedPinR,pwmR);
}