// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * pid-control.cpp -- PID control
 *                    based on Introduction to Feedback Control using
 *                    Design Studies by Randal W Beard, Timothy W.
 *                    McLain, Cammy Peterson, & Marc Killpack
 *                    ISBN-13: 978-1073396719
 *
 * Copyright (C) 2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include "pid-control.h"

#include <Arduino.h>


float PID_control::deadband_compensation(float unsat)
{
	if (!unsat) return unsat;

	return ((unsat / lowerLimit) * (lowerLimit - deadband_voltage_lower))
		+ (unsat < 0.0)
		? deadband_voltage_lower
		: deadband_voltage_upper;
}

PID_control::PID_control(float kp, float ki, float kd, float lowerLim, float upperLim, float sigma, float ts)
{
    this->kp         = kp;
    this->ki         = ki;
    this->kd         = kd;
    this->lowerLimit = lowerLimit;
    this->upperLimit = upperLimit;
    this->sigma      = sigma;
    this->ts         = ts;

    beta = ((2.0 * sigma) - ts) / ((2.0 * sigma) + ts);
}

PID_control::PID_control(float kp, float ki, float kd, float lowerLim, float upperLim, float sigma, float ts, bool errorDotEnabled, bool antiWindupEnabled)
{
	PID_control(kp, ki, kd, lowerLim, upperLim, sigma, ts);

	this->errorDotEnabled   = errorDotEnabled;
	this->antiWindupEnabled = antiWindupEnabled;
}

float PID_control::pd(float y_r, float y)
{
    float error;
    float u_unsat;

    error = y_r - y;

    // integrate error using the trapazoidal rule
    integrator = integrator + ((ts / 2.0) * (error + error_d1));

    // PD control
    if (errorDotEnabled) {
        error_dot = beta * error_dot + (((1.0 - beta) / ts) * (error - error_d1));
        u_unsat   = (kp * error) + (kd * error_dot);
    } else {
        y_dot   = beta * y_dot + (((1.0 - beta) / ts) * (y - y_d1));
        u_unsat = (kp * error) - (kd * y_dot);
    }

    error_d1 = error;
    y_d1     = y;

    return saturate(u_unsat);
}

float PID_control::pid(float y_r, float y)
{
    float error;
    float u_unsat;

    error = y_r - y;

    // integrate error using the trapazoidal rule
    integrator = integrator + ((ts / 2.0) * (error + error_d1));

    if(antiWindupEnabled && ki != 0.0){
	// generate an unsaturated signal from the integrator only and
	// then saturate the new integrator to the limit
        integrator = saturate(ki * integrator) / ki;
    }

    // PID control
    if (errorDotEnabled) {
        error_dot = beta * error_dot + (((1.0 - beta) / ts) * (error - error_d1));
        u_unsat   = (kp * error) + (ki * integrator) + (kd * error_dot);
    } else {
        y_dot   = beta * y_dot + (((1.0 - beta) / ts) * (y - y_d1));
        u_unsat = (kp*error) + (ki * integrator) - (kd * y_dot);
    }

    error_d1 = error;
    y_d1     = y;

    return deadband_compensation(saturate(u_unsat));
}

float PID_control::saturate(float unsat)
{
    return max(min(upperLimit, unsat), lowerLimit);
}

void PID_control::setDeadbands(float lower, float upper)
{
    deadband_voltage_lower = lower;
    deadband_voltage_upper = upper;
}

void PID_control::setGains(float kp, float ki, float kd)
{
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

void PID_control::setTimeParameters(float ts, float sigma)
{
    this->ts    = ts;
    this->sigma = sigma;

    beta = ((2.0 * sigma) - ts) / ((2.0 * sigma) + ts);
}

void PID_control::setpointReset(float y_r, float y)
{
    // reset the critical controller values to prevent an instant
    // setpoint change from ruining the controller response
    integrator = 0.0;
    error_d1   = y_r - y;
    error_dot  = 0.0;
}