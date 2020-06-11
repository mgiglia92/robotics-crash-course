#include <Arduino.h>

class State{
    public:
        float linear_state;  //A number representing if the robot should move forward/backward/stop
        float rotation_state; //A numebr representing robot turning setpoint, 0 represents no turning
    private:
        void setLinearState(float);
        void setRotationState(float);
        float getLinearState();
        float getRotationState();
};