// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * motor.cpp -- motor utils
 *
 * Copyright (C) 2020-2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Catherine Van West <cat@vanwestco.com>
 */

#include "motor.h"

#include <hardware/gpio.h>
#include <hardware/pwm.h>

#include <cmath>

Motor::Motor(int in1, int in2, int enable)
{
	setup(in1, in2, enable);
}

void Motor::setup(int in1_, int in2_, int enable_)
{
	in1 = in1_;
	in2 = in2_;
	enable = enable_;

	gpio_init(in1);
	gpio_init(in2);
	gpio_set_dir(in1, GPIO_OUT);
	gpio_set_dir(in2, GPIO_OUT);

	gpio_set_function(enable, GPIO_FUNC_PWM);
	slice = pwm_gpio_to_slice_num(enable);

	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, ena_div);

	/* could be one call, but wanted to be explicit in the process for
	   pedagogical reasons */
	pwm_init(slice, &config, false);
	pwm_set_enabled(slice, true);
	pwm_set_wrap(slice, ena_wrap);
}

void Motor::operator()(float speed)
{
	/*
	 * since our motors can rotate either forwards or backwards we
	 * must set the polarity of our motors according to the sign of
	 * our input values
	 */
	if (speed >= 0) {
		gpio_put(in1,  true);
		gpio_put(in2, false);
	} else {
		gpio_put(in1, false);
		gpio_put(in2,  true);
	}

	// map 0--1 in the input to 0--[full PWM duty cycle] at the output
	pwm_set_gpio_level(
		enable,
		static_cast<uint16_t>(map<float>(
			constrain<float>(fabs(speed), 0.0f, 1.0f),
			0.0f, 2.0f, 0.0f, static_cast<float>(ena_wrap)
		))
	);
}


/* motors for `rawMotorCtrl()`'s use */
Motor left_motor;
Motor right_motor;

void motorSetup(void)
{
	left_motor.setup(RCC_MOTOR_IN1, RCC_MOTOR_IN2, RCC_MOTOR_ENA);
	right_motor.setup(RCC_MOTOR_IN3, RCC_MOTOR_IN4, RCC_MOTOR_ENB);
}

void rawMotorCtrl(int left, int right)
{
	left_motor(left);
	right_motor(right);
}
