// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * encoder.h -- encoder interface
 *
 * Copyright (C) 2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_SENSOR_ENCODER_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_SENSOR_ENCODER_H


//Pin definitions for robot's encoder pins!
#define RIGHT_ENCODER 3
#define LEFT_ENCODER 4

class Odom{

public:
    Config();
    static volatile int right_count;
    static volatile int left_count;

    static void count_l();
    static void count_r();

    void setup_interrupts();
};

extern void cr();
extern void cl();


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_SENSOR_ENCODER_H */
