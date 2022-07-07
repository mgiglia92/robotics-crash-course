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
