// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * encoder.cpp -- encoder interface
 *
 * Copyright (C) 2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include "encoder.h"
#include "encoder_private.h"

#include <Arduino.h>


static volatile unsigned left_cnt;
static volatile unsigned right_cnt;


void encoderSetup(void)
{
	pinMode(RCC_LEFT_ENCODER_PIN,  INPUT);
	pinMode(RCC_RIGHT_ENCODER_PIN, INPUT);

	attachInterrupt(
		digitalPinToInterrupt(RCC_LEFT_ENCODER_PIN),
		left_encoder_isr,
		RISING
	);
	attachInterrupt(
		digitalPinToInterrupt(RCC_RIGHT_ENCODER_PIN),
		right_encoder_isr,
		RISING
	);
}

inline unsigned int getLeftEncoderCount(void)
{
	return left_cnt;
}

inline unsigned int getRightEncoderCount(void)
{
	return right_cnt;
}


static void left_encoder_isr(void)
{
	++left_cnt;
}

static void right_encoder_isr(void)
{
	++right_cnt;
}
