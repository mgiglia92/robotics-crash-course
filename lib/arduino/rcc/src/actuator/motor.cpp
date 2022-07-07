// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * motor.cpp -- motor utils
 *
 * Copyright (C) 2020-2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include "motor.h"

#include <Arduino.h>


void motorSetup(void)
{
	pinMode(RCC_MOTOR_ENA, OUTPUT);
	pinMode(RCC_MOTOR_ENB, OUTPUT);
	pinMode(RCC_MOTOR_IN1, OUTPUT);
	pinMode(RCC_MOTOR_IN2, OUTPUT);
	pinMode(RCC_MOTOR_IN3, OUTPUT);
	pinMode(RCC_MOTOR_IN4, OUTPUT);
}

void rawMotorCtrl(int left, int right)
{
	/*
	 * since our motors can rotate either forwards or backwards we
	 * must set the polarity of our motors according to the sign of
	 * our input values
	 */

	if (left >= 0) {
		digitalWrite(RCC_MOTOR_IN1, HIGH);
		digitalWrite(RCC_MOTOR_IN2,  LOW);
	} else {
		digitalWrite(RCC_MOTOR_IN1,  LOW);
		digitalWrite(RCC_MOTOR_IN2, HIGH);
	}

	if (right >= 0) {
		digitalWrite(RCC_MOTOR_IN3, HIGH);
		digitalWrite(RCC_MOTOR_IN4,  LOW);
	} else {
		digitalWrite(RCC_MOTOR_IN3,  LOW);
		digitalWrite(RCC_MOTOR_IN4, HIGH);
	}

	analogWrite(RCC_MOTOR_ENA, constrain(left,  0, 255));
	analogWrite(RCC_MOTOR_ENB, constrain(right, 0, 255));
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
