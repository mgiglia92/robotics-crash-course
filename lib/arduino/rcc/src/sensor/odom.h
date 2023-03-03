// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * odom.h -- encoder interface
 *
 * Copyright (C) 2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Catherine Van West <cat@vanwestco.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_ODOM_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_ODOM_H


#define RCC_LEFT_ENCODER_PIN_INT  2
#define RCC_LEFT_ENCODER_PIN_DIR  1
#define RCC_RIGHT_ENCODER_PIN_INT 3
#define RCC_RIGHT_ENCODER_PIN_DIR 4


template<int left_int_pin,
         int right_int_pin>
class Odom {
private:
	static void left_encoder_isr(void);
	static void right_encoder_isr(void);

protected:
	static volatile unsigned long left_cnt;
	static volatile unsigned long right_cnt;

	void attach(void (*left_isr)(void), void (*right_isr)(void));

public:
	Odom();

	unsigned long getLeftCount(void);
	unsigned long getRightCount(void);
};

template<int left_int_pin,
         int right_int_pin,
         int left_dir_pin,
         int right_dir_pin>
class Directional_Odom : public Odom<left_int_pin, right_int_pin> {
private:
	static void left_encoder_isr(void);
	static void right_encoder_isr(void);

protected:
	using Odom<left_int_pin, right_int_pin>::left_cnt;
	using Odom<left_int_pin, right_int_pin>::right_cnt;

public:
	Directional_Odom();
};

using Default_Odom = Odom<RCC_LEFT_ENCODER_PIN_INT,
                          RCC_RIGHT_ENCODER_PIN_INT>;

using Default_Directional_Odom = Directional_Odom<RCC_LEFT_ENCODER_PIN_INT,
                                                  RCC_RIGHT_ENCODER_PIN_INT,
                                                  RCC_LEFT_ENCODER_PIN_DIR,
                                                  RCC_RIGHT_ENCODER_PIN_DIR>;


#include "odom_impl.h"


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_ODOM_H */
