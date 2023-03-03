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


template<int left_int_pin,
         int right_int_pin>
static volatile unsigned long Odom<left_int_pin, right_int_pin>::left_cnt = 0;

template<int left_int_pin,
         int right_int_pin>
static volatile unsigned long Odom<left_int_pin, right_int_pin>::right_cnt = 0;

template<int left_int_pin,
         int right_int_pin>
Odom<left_int_pin, right_int_pin>::Odom()
{
	pinMode(left_int_pin,  INPUT);
	pinMode(right_int_pin, INPUT);
	attach(left_encoder_isr, right_encoder_isr);
}

template<int left_int_pin,
         int right_int_pin>
void Odom<left_int_pin, right_int_pin>::attach(void (*left_isr)(void),
                                               void (*right_isr)(void))
{
	attachInterrupt(
		digitalPinToInterrupt(left_int_pin),
		left_isr,
		CHANGE
	);
	attachInterrupt(
		digitalPinToInterrupt(right_int_pin),
		right_isr,
		CHANGE
	);
}

template<int left_int_pin,
         int right_int_pin>
void Odom<left_int_pin, right_int_pin>::left_encoder_isr(void)
{
	++left_cnt;
}

template<int left_int_pin,
         int right_int_pin>
void Odom<left_int_pin, right_int_pin>::right_encoder_isr(void)
{
	++right_cnt;
}

template<int left_int_pin,
         int right_int_pin>
unsigned long Odom<left_int_pin, right_int_pin>::getLeftCount(void)
{
	return left_cnt;
}

template<int left_int_pin,
         int right_int_pin>
unsigned long Odom<left_int_pin, right_int_pin>::getRightCount(void)
{
	return right_cnt;
}


template<int left_int_pin,
         int right_int_pin,
         int left_dir_pin,
         int right_dir_pin>
Directional_Odom<left_int_pin, right_int_pin, left_dir_pin, right_dir_pin>
::Directional_Odom()
{
	pinMode(left_dir_pin, INPUT);
	pinMode(right_dir_pin, INPUT);
	attach(left_encoder_isr, right_encoder_isr);
}

template<int left_int_pin,
         int right_int_pin,
         int left_dir_pin,
         int right_dir_pin>
void Directional_Odom<left_int_pin, right_int_pin, left_dir_pin, right_dir_pin>
::left_encoder_isr(void)
{
	const bool pin_1 = digitalRead(left_int_pin);
	const bool pin_2 = digitalRead(left_dir_pin);

	if (pin_1 ^ pin_2) --left_cnt;
	else ++left_cnt;
}

template<int left_int_pin,
         int right_int_pin,
         int left_dir_pin,
         int right_dir_pin>
void Directional_Odom<left_int_pin, right_int_pin, left_dir_pin, right_dir_pin>
::right_encoder_isr(void)
{
	const bool pin_1 = digitalRead(right_int_pin);
	const bool pin_2 = digitalRead(right_dir_pin);

	if (pin_1 ^ pin_2) --right_cnt;
	else ++right_cnt;
}


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_ODOM_IMPL_H */
