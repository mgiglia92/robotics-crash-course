#include <Arduino.h>

class State{
    public:
        State();
        int linear_state;  //A number representing if the robot should move forward/backward/stop
        int rotation_state; //A numebr representing robot turning setpoint, 0 represents no turning
        void setLinearState(int);
        void setRotationState(int);
        int getLinearState();
        int getRotationState();
};