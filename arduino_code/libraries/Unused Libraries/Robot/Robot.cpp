#include <Robot.h>

Robot::Robot(PID linear, PID rotation, ControllerParams controller_params)
{
    cp = controller_params;    
    linear_controller = linear;
    rotation_controller = rotation_controller;


}

//Initialize important parameters
void Robot::initialize()
{
    //Initiailize controller 

    //Initialize other Linear controller parameters
    linear_controller.SetOutputLimits(-100, 100);
    linear_controller.SetSampleTime(25);
    linear_controller.SetMode(1);
    //Initialize other Rotation controller parameters
    rotation_controller.SetOutputLimits(-100, 100);
    rotation_controller.SetSampleTime(25);
    rotation_controller.SetMode(1);
    //TODO: Initialize MPU6050
    //TODO: Initialize HC_SR04
}

void Robot::update()
{
    //TODO: Update routine
}

void run()
{
    while(1)
    {
        //TODO: Do robot algorithm here
    }
}