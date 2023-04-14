// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * differentiator.h -- differentiator
 *
 * Copyright (C) 2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_UTIL_DIFFERENTIATOR_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_UTIL_DIFFERENTIATOR_H


class Differentiator {
private:
	float sigma;
	float ts;
	float beta;

	float y_dot = 0.0;
	float y_d1  = 0.0;

public:
	Differentiator(float sigma, float ts);

	float differentiate(float y);
	void reset(float degrees);
	void setTimeParameters(float ts, float sigma);
};


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_UTIL_DIFFERENTIATOR_H */
