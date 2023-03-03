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


Directional_Odom *Directional_Odom::instance = nullptr;


static void Directional_Odom::left_encoder_isr(void)
{
	const bool pin_1 = digitalRead(RCC_LEFT_ENCODER_PIN);
	const bool pin_2 = digitalRead(RCC_LEFT_ENCODER_PIN_2);

	if (pin_1 ^ pin_2) --instance->left_cnt;
	else ++instance->left_cnt;
}

static void Directional_Odom::right_encoder_isr(void)
{
	const bool pin_1 = digitalRead(RCC_RIGHT_ENCODER_PIN);
	const bool pin_2 = digitalRead(RCC_RIGHT_ENCODER_PIN_2);

	if (pin_1 ^ pin_2) --instance->right_cnt;
	else ++instance->right_cnt;
}

void Directional_Odom::begin(void)
{
	if (instance) return;
	instance = this;

	pinMode(RCC_LEFT_ENCODER_PIN,  INPUT);
	pinMode(RCC_RIGHT_ENCODER_PIN, INPUT);

	attachInterrupt(
		digitalPinToInterrupt(RCC_LEFT_ENCODER_PIN),
		left_encoder_isr,
		CHANGE
	);
	attachInterrupt(
		digitalPinToInterrupt(RCC_RIGHT_ENCODER_PIN),
		right_encoder_isr,
		CHANGE
	);
}

unsigned long Directional_Odom::getLeftCount(void)
{
	return left_cnt;
}

unsigned long Directional_Odom::getRightCount(void)
{
	return right_cnt;
}
