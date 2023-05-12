// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * motor.h -- motor utils
 *
 * Copyright (C) 2020-2022  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2020  Andrew Lorber <andrewlorber@aol.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Catherine Van West <cat@vanwestco.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_ACTUATOR_MOTOR_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_ACTUATOR_MOTOR_H


#include <pico/stdlib.h>


#define RCC_MOTOR_IN1 40
#define RCC_MOTOR_IN2 42
#define RCC_MOTOR_ENA 44
#define RCC_MOTOR_ENB 46
#define RCC_MOTOR_IN3 48
#define RCC_MOTOR_IN4 50


template <typename numeric_t>
numeric_t constrain(numeric_t input, numeric_t lower, numeric_t upper)
{
	if (input < lower) return lower;
	if (input > upper) return upper;
	return input;
}

template <typename numeric_t>
numeric_t map(numeric_t input,
              numeric_t in_low,
              numeric_t in_high,
              numeric_t out_low,
              numeric_t out_high)
{
	const numeric_t io_ratio = (out_high - out_low)/(in_high - in_low);
	return out_low + (input - in_low)*io_ratio;
}

#define pwr2pwm(PWR) map<int>(constrain<int>(PWR, 0, 100), 0, 100, 0, 255)


class Motor {
private:
	/* divide the Pico's 125 MHz clock down to 125e6/2^16/16 ≈ 120 Hz */
	static constexpr const uint16_t ena_wrap = 0xFFFF;
	static constexpr const float ena_div = 16.0f;

	int in1, in2, enable;
	uint slice;

public:
	Motor() = default;
	Motor(int in1, int in2, int enable);

	void setup(int in1, int in2, int enable);
	void operator()(float speed);
};


void motorSetup(void);

/*
 * control the direction of motors using signed pwm values
 */
void rawMotorCtrl(int left, int right);


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_ACTUATOR_MOTOR_H */
