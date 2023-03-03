// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * odom.h -- encoder interface
 *
 * Copyright (C) 2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_ODOM_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_ODOM_H


#define RCC_LEFT_ENCODER_PIN  2
#define RCC_RIGHT_ENCODER_PIN 3


class Odom {
private:
	static Odom *instance;

	volatile unsigned long left_cnt;
	volatile unsigned long right_cnt;

	static void left_encoder_isr(void);
	static void right_encoder_isr(void);

public:
	void begin(void);
	unsigned long getLeftCount(void);
	unsigned long getRightCount(void);
};


#define RCC_LEFT_ENCODER_PIN_2 1
#define RCC_RIGHT_ENCODER_PIN_2 4


class Directional_Odom {
private:
	static Directional_Odom *instance;

	volatile unsigned long left_cnt;
	volatile unsigned long right_cnt;

	static void left_encoder_isr(void);
	static void right_encoder_isr(void);

public:
	void begin(void);
	unsigned long getLeftCount(void);
	unsigned long getRightCount(void);
};


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_ODOM_H */
