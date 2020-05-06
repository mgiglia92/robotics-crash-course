#include "GY_521.h"

//Default constructor
GY_521::GY_521(int sda, int scl)
{
    //Do nothing
}

//Read all data from sensor, return true if it completes

bool GY_521::update()
{
    //TODO: Do update method here
    _ax = 10.1;
    _ay = 5.2;
    _az = 5.5;
    //etc...
    return true;
}

//Get acceleration value, choose x,y,z
float GY_521::get_accel(char c)
{
    if(c=='x'){ return _ax;}
    if(c=='y'){ return _ay;}
    if(c=='z'){ return _az;}
    else { return -999999; }
}

//Get angular velocity value, choost x,y,z
float GY_521::get_ang_vel(char c)
{
    if(c=='x'){ return _wx;}
    if(c=='y'){ return _wy;}
    if(c=='z'){ return _wz;}    
    else{ return -999999; }
}

//Get temperature value
float GY_521::get_temp() { return temp; }
