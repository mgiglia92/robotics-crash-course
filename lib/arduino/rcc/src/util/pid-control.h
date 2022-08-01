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
public:
    float kp; //Proportional gain
    float ki; //Integral gain
    float kd; //Derivative gain
    float lowerLimit; //Output saturates at this limit on lower end
    float upperLimit; //Output saturates at this limit on upper end
    float sigma; //dirty derivative bandwidth = 1/sigma
    float ts; //sample period in seconds
    float beta; //(2.0*sigma-ts)/(2.0*sigma+ts)
    bool flag; //derivative on error_dot(true) or y_dot(false)
    float y_dot; //estimated derivative of y
    float y_d1; //signal y delayed by one sample
    float error_dot; //estimated derivative of error
    float error_d1; //error delayed by one sample
    float integrator; //integrator value
    float integrator_unsat; //Unsaturated integrator value
    float integrator_sat; //Integrator value saturated
    int anti_windup_activated; //activate anti-windup
    float deadband_voltage_upper; //Voltage at which motor starts to rotate
    float deadband_voltage_lower;

    PID_control(float, float, float, float, float, float, float, bool);
    float PID(float,float);
    float PD(float, float);
    float saturate(float);
    void update_time_parameters(float, float);
    void update_gains(float, float, float);
    void setpoint_reset(float, float);
    void update_deadband_values(float, float);
    float deadband_compensation(float);
};


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_UTIL_PID_CONTROL_H */
