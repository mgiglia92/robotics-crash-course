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


//  Sets up motors - sets pins
void motor_setup();

// Conversion utilities
int convert_pwr_to_pwm(int pwr);

// ------  Motor Control Functions ------

// pwr should always be 0-100
// commands: 0 - Forward
//           1 - Backward
//           2 - Right
//           3 - Left
void control(int command, int pwr);

// Cuts power to all motors
void dwell();

// Moves foward
void backward(int pwm);

// Moves backward
void forward(int pwm);

// Rotates left using differential drive style
void diff_left(int pwm);

// Rotates right using differential drive style
void diff_right(int pwm);

//control motors directly
void raw_motor_control(int, int);


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_ACTUATOR_MOTOR_H */
