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


PID_control::PID_control(float p, float i, float d, float llim, float ulim, float sigm, float t, bool fl){

    kp = p;
    ki = i;
    kd = d;
    lowerLimit = llim;
    upperLimit = ulim;
    sigma = sigm;
    Ts = t;
    flag = fl;
    beta = (2.0*sigma - Ts) / (2.0*sigma + Ts);

    //Initialize delayed values to zero
    y_d1 = 0;
    error_d1 = 0;

    //Initialize derivative values to zero
    y_dot = 0;
    error_dot = 0;

    //Initialize integrator value to zero
    integrator = 0;

    // Inialize deadband voltages
    deadband_voltage_upper = 0;
    deadband_voltage_lower = 0;
}


//PID calculation
float PID_control::PID(float y_r, float y){
    //Initialize variables to prevent compiler errors
    float u_unsat;

    //Compute the current error
    float error = y_r - y;

    //Integrate errkor using trapazoidal rule
    integrator = integrator + ((Ts/2) * (error + error_d1));

    if(anti_windup_activated==1 && ki != 0){
        //Generate unsaturated signal from integrator only
        integrator_unsat = ki*integrator;

        //Saturate the integrator to the limit
        integrator = saturate(integrator_unsat)/ki;
    }

    //PID control
    if(flag == true){
        //Differentiate error
        error_dot = beta * error_dot + (((1 - beta)/Ts) * (error - error_d1));

        //PID control
        u_unsat = (kp*error) + (ki * integrator) + (kd * error_dot);
    }

    else{
        //differentiate y
        y_dot = beta * y_dot + (((1 - beta) / Ts) * (y - y_d1));

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
    integrator = integrator + ((Ts/2) * (error + error_d1));

    //PID control
    if(flag == true){
        //Differentiate error
        error_dot = beta * error_dot + (((1 - beta)/Ts) * (error - error_d1));

        //PID control
        u_unsat = (kp*error) + (kd * error_dot);
    }

    else{
        //differentiate y
        y_dot = beta * y_dot + (((1 - beta) / Ts) * (y - y_d1));

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

//Saturation check (from beard)
// float PID_control::saturate(float u){
//     //Check if absolute value is above limit, clip value if so
//     if (abs(u) > limit){
//         u = limit * (abs(u) / u);
//     }
//     return u;
// }

//Saturation considering two bounds not equal to each other
float PID_control::saturate(float u){
    return max(min(upperLimit, u), lowerLimit);
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
    Ts = t;
    sigma = s;
    beta = (2.0*sigma - Ts) / (2.0*sigma + Ts);
}

void PID_control::update_gains(float p, float i, float d){
    kp = p;
    ki = i;
    kd = d;
}

void PID_control::setpoint_reset(float y_r, float y){
    // Reset the critical controller values to prevent instant setpoint change from
    // ruining the response
    integrator = 0;
    error_d1 = y_r - y;
    error_dot = 0;
}

void PID_control::update_deadband_values(float upper, float lower){
    deadband_voltage_upper = upper;
    deadband_voltage_lower = lower;
}
