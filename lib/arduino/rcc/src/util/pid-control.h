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
    double kp; //Proportional gain
    double ki; //Integral gain
    double kd; //Derivative gain
    double lowerLimit; //Output saturates at this limit on lower end
    double upperLimit; //Output saturates at this limit on upper end
    double sigma; //dirty derivative bandwidth = 1/sigma
    double Ts; //sample period in seconds
    double beta; //(2.0*sigma-Ts)/(2.0*sigma+Ts)
    bool flag; //derivative on error_dot(true) or y_dot(false)
    double y_dot; //estimated derivative of y
    double y_d1; //signal y delayed by one sample
    double error_dot; //estimated derivative of error
    double error_d1; //error delayed by one sample
    double integrator; //integrator value
    double integrator_unsat; //Unsaturated integrator value
    double integrator_sat; //Integrator value saturated
    int anti_windup_activated; //activate anti-windup
    double deadband_voltage_upper; //Voltage at which motor starts to rotate
    double deadband_voltage_lower;

    PID_control(double, double, double, double, double, double, double, bool);
    double PID(double,double);
    double PD(double, double);
    double saturate(double);
    void update_time_parameters(double, double);
    void update_gains(double, double, double);
    void setpoint_reset(double, double);
    void update_deadband_values(double, double);
    double deadband_compensation(double);
};


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_UTIL_PID_CONTROL_H */
