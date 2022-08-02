// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * differentiator.h -- differentiator
 *
 * Copyright (C) 2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_UTIL_DIFFERENTIATOR_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_UTIL_DIFFERENTIATOR_H


class differentiator{

public:
    //Constructors
    differentiator(float, float);

    //members
    float sigma; //dirty derivative bandwidth = 1/sigma
    float ts; //sample period in sec
    float beta; //(2.0*sigma-ts)/(2.0*sigma+ts)


    //methods
    float differentiate(float);
    void update_time_parameters(float, float);
    void reset(float);

private:
    float y_d1; //differentiation variable 1 sample behind
    float y_dot; //derivative of y
};


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_UTIL_DIFFERENTIATOR_H */
