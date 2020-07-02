#ifndef CONTROLLERPARAMS_H
#define CONTROLLERPARAMS_H

#include <Arduino.h>

class ControllerParams{
    public:
        ControllerParams(int);
        double kpl;
        double kil;
        double kdl;
        double linear_input;
        double linear_output;
        double linear_setpoint;
        double kpr;
        double kir;
        double kdr;
        double rotation_input;
        double rotation_output;
        double rotation_setpoint;
};

class test{
    public:
        test();
        int val;
};
#endif