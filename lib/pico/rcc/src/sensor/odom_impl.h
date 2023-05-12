// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * odom_impl.h -- encoder interface implementation
 *
 * Copyright (C) 2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Catherine Van West <cat@vanwestco.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_ODOM_IMPL_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_ODOM_IMPL_H


#include <Arduino.h>


template<int int_pin>
static volatile long Odom<int_pin>::count = 0;

template<int int_pin>
Odom<int_pin>::Odom()
: sign(1) {
	pinMode(int_pin,  INPUT);
	attach(encoder_isr);
}

template<int int_pin>
void Odom<int_pin>::attach(void (*isr)(void))
{
	attachInterrupt(
		digitalPinToInterrupt(int_pin),
		isr,
		CHANGE
	);
}

template<int int_pin>
void Odom<int_pin>::encoder_isr(void)
{
	++count;
}

template<int int_pin>
long Odom<int_pin>::getCount(void)
{
	return sign*count;
}

template<int int_pin>
void Odom<int_pin>::setSign(int sign_)
{
	if (sign_ >= 0) sign = 1;
	else sign = -1;
}

template<int int_pin>
void Odom<int_pin>::setZero()
{
	count = 0;
}

template<int int_pin>
void Odom<int_pin>::setZero(long zero)
{
	count -= zero;
}

template<int int_pin, int dir_pin>
Dir_Odom<int_pin, dir_pin>::Dir_Odom()
{
	pinMode(dir_pin, INPUT);
	attach(encoder_isr);
}

template<int int_pin, int dir_pin>
void Dir_Odom<int_pin, dir_pin>::encoder_isr()
{
	const bool pin_1 = digitalRead(int_pin);
	const bool pin_2 = digitalRead(dir_pin);

	if (pin_1 ^ pin_2) --count;
	else ++count;
}

#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_ODOM_IMPL_H */
