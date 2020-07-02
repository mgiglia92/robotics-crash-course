#include <Arduino.h>
#include "ControllerParams.h"
#include "PID_v1.h"

class Robot{
    public:
        Robot(PID, PID, ControllerParams);
        void run();
        void update();
        void initialize();
    private:
        ControllerParams cp;
        PID linear_controller;//(double, double, double, double, double, double, int);
        PID rotation_controller;//(double, double, double, double, double, double, int);

};