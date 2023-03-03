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


template<int int_pin>
class Odom {
private:
	static void encoder_isr(void);

protected:
	static volatile long count;
	int sign;

	void attach(void (*isr)(void));

public:
	Odom();

	long getCount(void);
	void setSign(int sign);
	void setZero();
	void setZero(long zero);
};

template<int int_pin, int dir_pin>
class Dir_Odom : public Odom<int_pin> {
private:
	static void encoder_isr(void);

protected:
	using Odom<int_pin>::count;

public:
	Dir_Odom();
};

using Left_Odom = Odom<RCC_LEFT_ENCODER_PIN_INT>;
using Right_Odom = Odom<RCC_RIGHT_ENCODER_PIN_INT>;
using Left_Dir_Odom = Dir_Odom<RCC_LEFT_ENCODER_PIN_INT,
                               RCC_LEFT_ENCODER_PIN_DIR>;
using Right_Dir_Odom = Dir_Odom<RCC_RIGHT_ENCODER_PIN_INT,
                                RCC_RIGHT_ENCODER_PIN_DIR>;


#include "odom_impl.h"


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_ODOM_H */
