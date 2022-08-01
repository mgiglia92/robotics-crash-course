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

void PID_control::setpointReset(float y_r, float y)
{
    // reset the critical controller values to prevent an instant
    // setpoint change from ruining the controller response
    integrator = 0.0;
    error_d1   = y_r - y;
    error_dot  = 0.0;
}

//PID calculation
float PID_control::PID(float y_r, float y){
    //Initialize variables to prevent compiler errors
    float u_unsat;

    //Compute the current error
    float error = y_r - y;

    //Integrate errkor using trapazoidal rule
    integrator = integrator + ((ts/2) * (error + error_d1));

    if(antiWindupEnabled && ki != 0){
        //Generate unsaturated signal from integrator only
        integrator_unsat = ki*integrator;

        //Saturate the integrator to the limit
        integrator = saturate(integrator_unsat)/ki;
    }

    //PID control
    if(errorDotEnabled) {
        //Differentiate error
        error_dot = beta * error_dot + (((1 - beta)/ts) * (error - error_d1));

        //PID control
        u_unsat = (kp*error) + (ki * integrator) + (kd * error_dot);
    }

    else{
        //differentiate y
        y_dot = beta * y_dot + (((1 - beta) / ts) * (y - y_d1));

        //PID control
        u_unsat = (kp*error) + (ki * integrator) - (kd * y_dot);
    }

    //Return saturated control signal
    float u_sat = saturate(u_unsat);



    //Integrator anti-windup beard
    // if(ki != 0.0){
    //     integrator = integrator + ((1.0 / ki) * (u_sat - u_unsat));
    // }

    //Update delayed variables
    error_d1 = error;
    y_d1 = y;
    return deadband_compensation(u_sat);
}

//PD calculation
float PID_control::PD(float y_r, float y){
//Initialize variables to prevent compiler errors
    float u_unsat;

    //Compute the current error
    float error = y_r - y;

    //Integrate errkor using trapazoidal rule
    integrator = integrator + ((ts/2) * (error + error_d1));

    //PID control
    if(errorDotEnabled) {
        //Differentiate error
        error_dot = beta * error_dot + (((1 - beta)/ts) * (error - error_d1));

        //PID control
        u_unsat = (kp*error) + (kd * error_dot);
    }

    else{
        //differentiate y
        y_dot = beta * y_dot + (((1 - beta) / ts) * (y - y_d1));

        //PID control
        u_unsat = (kp*error) - (kd * y_dot);
    }

    //Return saturated control signal
    float u_sat = saturate(u_unsat);

    //Update delayed variables
    error_d1 = error;
    y_d1 = y;
    return u_sat;
}

// Compensate for motor deadband, by adjusting output related to deadband voltages.
float PID_control::deadband_compensation(float u){
    if(u > 0){
        return deadband_voltage_upper + ( (u/upperLimit) * (upperLimit - deadband_voltage_upper) );
    }
    if(u < 0){
        return deadband_voltage_lower + ( (u/lowerLimit) * (lowerLimit - deadband_voltage_lower) );
    }
    else{ return u; }
}

void PID_control::update_time_parameters(float t, float s){
    ts = t;
    sigma = s;
    beta = (2.0*sigma - ts) / (2.0*sigma + ts);
}
