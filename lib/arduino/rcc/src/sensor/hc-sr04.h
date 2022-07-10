// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * hc-sr04.h -- HC-SR04 interface
 *
 * Copyright (C) 2020-2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2020  Andrew Lorber <andrewlorber@aol.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_SENSOR_HC_SR04_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_SENSOR_HC_SR04_H


#define RCC_ECHO_PIN 4
#define RCC_TRIG_PIN 10

#define RCC_ULTRASONIC_TIMEOUT_US 5000


class HC_SR04 {
  public:
    // Params are pin numbers & max distance
    HC_SR04(int trigger, int echo, int interrupt, int max_dist=200);
    // Initializes pins
    void initialize();
    // Runs Sonar sensor
    void start();
    bool isFinished(){ return _finished; }
    double getRangeReset(); //Update distance member, start next reading if updated, if not return old value
    unsigned int getRange(); //Return distance calculated directly. Need to check if finished before calling this fucntion
    static HC_SR04* instance(){ return _instance; }

  private:
    static void _echo_isr();
    double distance;
    int _trigger, _echo, _int, _max;
    volatile unsigned long _start, _end;
    volatile bool _finished;
    static HC_SR04* _instance;
};


void ultrasonicAsyncSetup(void);
void ultrasonicAsyncPulse(unsigned long timeout_us = RCC_ULTRASONIC_TIMEOUT_US);
bool ultrasonicAsyncPulseDone(void);
unsigned long ultrasonicPulse(unsigned long timeout_us = RCC_ULTRASONIC_TIMEOUT_US);
void ultrasonicSetup(void);


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_SENSOR_HC_SR04_H */
