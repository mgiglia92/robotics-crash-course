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


typedef struct PID_control_config_s {
	float kp; //Proportional gain
	float ki; //Integral gain
	float kd; //Derivative gain
	float lowerLimit; //Lower limit of controller output
	float upperLimit; //Upper limit of controller output
	float sigma; //Bandwidth for the bandlimited derivative calculation (rad/sec)
	float ts; //The sample time in seconds
	bool  errorDotEnabled; // If true use calc derivative based off of the error instead of the state
	bool  antiWindupEnabled; //If true, enable simple integral antiwind up (clipping style)
} PID_control_config_t;


class PID_control {
private:
	float kp;
	float ki;
	float kd;
	float lowerLimit;
	float upperLimit;
	float sigma;
	float ts;
	float beta;

	float deadband_voltage_lower = 0.0;
	float deadband_voltage_upper = 0.0;
	float integrator             = 0.0;
	float error_dot              = 0.0;
	float error_d1               = 0.0;
	float y_dot                  = 0.0;
	float y_d1                   = 0.0;

	float deadband_compensation(float unsat);
	float saturate(float unsat);

public:
	bool errorDotEnabled   = false;
	bool antiWindupEnabled = false;

	PID_control(float kp, float ki, float kd, float lowerLimit, float upperLimit, float sigma, float ts);
	PID_control(float kp, float ki, float kd, float lowerLimit, float upperLimit, float sigma, float ts, bool errorDotEnabled, bool antiWindupEnabled);
	PID_control(const PID_control_config_t &config);

	float pd(float y_r, float y);
	float pid(float y_r, float y);
	void setDeadbands(float lower, float upper);
	void setGains(float kp, float ki, float kd);
	void setTimeParameters(float ts, float sigma);
	void setpointReset(float y_r, float y);
};


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_UTIL_PID_CONTROL_H */
