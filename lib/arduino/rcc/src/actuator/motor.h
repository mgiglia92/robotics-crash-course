// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * motor.h -- motor utils
 *
 * Copyright (C) 2020-2022  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2020  Andrew Lorber <andrewlorber@aol.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_ACTUATOR_MOTOR_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_ACTUATOR_MOTOR_H


#define RCC_MOTOR_ENA 5
#define RCC_MOTOR_ENB 6
#define RCC_MOTOR_IN1 7
#define RCC_MOTOR_IN2 8
#define RCC_MOTOR_IN3 11
#define RCC_MOTOR_IN4 12

#define pwr2pwm(PWR) map(PWR, 0, 100, 0, 255)


void motorSetup(void);

/*
 * control the direction of motors using signed pwm values
 */
void rawMotorCtrl(int left, int right);


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_ACTUATOR_MOTOR_H */
