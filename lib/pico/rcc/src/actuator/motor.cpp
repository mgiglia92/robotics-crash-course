// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * motor.cpp -- motor utils
 *
 * Copyright (C) 2020-2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include "motor.h"

#include <Arduino.h>


Motor::Motor(int in1, int in2, int enable)
{
	setup(in1, in2, enable);
}

void Motor::setup(int in1_, int in2_, int enable_)
{
	in1 = in1_;
	in2 = in2_;
	enable = enable_;

	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(enable, OUTPUT);
}

void Motor::operator()(int speed)
{
	/*
	 * since our motors can rotate either forwards or backwards we
	 * must set the polarity of our motors according to the sign of
	 * our input values
	 */
	if (speed >= 0) {
		digitalWrite(in1, HIGH);
		digitalWrite(in2,  LOW);
	} else {
		digitalWrite(in1,  LOW);
		digitalWrite(in2, HIGH);
	}

	analogWrite(enable, constrain(abs(speed),  0, 255));
}


/* motors for `rawMotorCtrl()`'s use */
Motor left_motor;
Motor right_motor;

void motorSetup(void)
{
	left_motor.setup(RCC_MOTOR_IN1, RCC_MOTOR_IN2, RCC_MOTOR_ENA);
	right_motor.setup(RCC_MOTOR_IN3, RCC_MOTOR_IN4, RCC_MOTOR_ENB);
}

void rawMotorCtrl(int left, int right)
{
	left_motor(left);
	right_motor(right);
}
