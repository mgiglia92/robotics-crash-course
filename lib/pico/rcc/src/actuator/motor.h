// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * motor.h -- motor utils
 *
 * Copyright (C) 2020-2022  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2020  Andrew Lorber <andrewlorber@aol.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_ACTUATOR_MOTOR_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_ACTUATOR_MOTOR_H


#define RCC_MOTOR_IN1 40
#define RCC_MOTOR_IN2 42
#define RCC_MOTOR_ENA 44
#define RCC_MOTOR_ENB 46
#define RCC_MOTOR_IN3 48
#define RCC_MOTOR_IN4 50

#define pwr2pwm(PWR) map(constrain(PWR, 0, 100), 0, 100, 0, 255)


class Motor {
private:
	int in1, in2, enable;

public:
	Motor() = default;
	Motor(int in1, int in2, int enable);

	void setup(int in1, int in2, int enable);
	void operator()(int speed);
};


void motorSetup(void);

/*
 * control the direction of motors using signed pwm values
 */
void rawMotorCtrl(int left, int right);


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_ACTUATOR_MOTOR_H */
