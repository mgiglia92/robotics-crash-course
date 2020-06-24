#include "State.h"

State::State()
{
    linear_state = 0;
    rotation_state = 0;
}

void State::setLinearState(int val) { linear_state=val;}
void State::setRotationState(int val) { rotation_state=val;}

int State::getLinearState(){return linear_state;}
int State::getRotationState(){return rotation_state;}