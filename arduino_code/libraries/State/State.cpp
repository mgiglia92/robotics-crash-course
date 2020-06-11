#include "State.h"

State::State()
{
    linear_state = 0.0;
    rotation_state = 0.0;
}

void State::setLinearState(float val) { linear_state=val;}
void State::setRotationState(float val) { rotation_state=val;}

float State::getLinearState(){return linear_state;}
float State::getRotationState(){return rotation_state;}