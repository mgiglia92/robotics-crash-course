// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * pid-control.h -- PID control
 *                  based on Introduction to Feedback Control using
 *                  Design Studies by Randal W Beard, Timothy W. McLain,
 *                  Cammy Peterson, & Marc Killpack
 *                  ISBN-13: 978-1073396719
 *
 * Copyright (C) 2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_UTIL_PID_CONTROL_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_UTIL_PID_CONTROL_H


class PID_control {
private:
	float deadband_voltage_lower = 0.0;
	float deadband_voltage_upper = 0.0;
	float integrator             = 0.0;
	float integrator_unsat       = 0.0;
	float error_dot              = 0.0;
	float error_d1               = 0.0;
	float y_dot                  = 0.0;
	float y_d1                   = 0.0;

public:
	float kp;
	float ki;
	float kd;
	float lowerLimit;
	float upperLimit;
	float sigma;
	float ts;
	float beta;

	bool errorDotEnabled   = false;
	bool antiWindupEnabled = false;

	PID_control(float kp, float ki, float kd, float lowerLim, float upperLim, float sigma, float ts);
	PID_control(float kp, float ki, float kd, float lowerLim, float upperLim, float sigma, float ts, bool errorDotEnabled, bool antiWindupEnabled);

	float saturate(float unsat);

    float PID(float,float);
    float PD(float, float);
    void update_time_parameters(float, float);
    void update_gains(float, float, float);
    void setpoint_reset(float, float);
    void update_deadband_values(float, float);
    float deadband_compensation(float);
};


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_UTIL_PID_CONTROL_H */
