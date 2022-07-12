// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * odom.cpp -- encoder interface
 *
 * Copyright (C) 2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include "odom.h"

#include <Arduino.h>


Odom *Odom::instance = nullptr;


static void Odom::left_encoder_isr(void)
{
	++instance->left_cnt;
}

static void Odom::right_encoder_isr(void)
{
	++instance->right_cnt;
}

void Odom::begin(void)
{
	if (instance) return;
	instance = this;

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

unsigned long Odom::getLeftCount(void)
{
	return left_cnt;
}

unsigned long Odom::getRightCount(void)
{
	return right_cnt;
}
