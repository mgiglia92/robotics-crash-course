#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

//Pin definitions for robot's encoder pins!
#define RIGHT_ENCODER 3
#define LEFT_ENCODER 4

class Odom{

public:
    Config();
    static volatile int right_count;
    static volatile int left_count;

    static void count_l();
    static void count_r();

    void setup_interrupts();
};

extern void cr();
extern void cl();