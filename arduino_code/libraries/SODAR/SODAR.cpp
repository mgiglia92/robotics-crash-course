#include <SODAR.h>

SODAR::SODAR(int trig_pin,
            int echo_pin,
            int echo_int,
            int servo_pin,
            int cen, 
            int res, 
            int uprange, 
            int lowrange)
{
    center = cen;
    resolution = res;
    upper_range = uprange;
    lower_range = lowrange;
    num_upper_segments = (uprange - center)/resolution;
    num_lower_segments = (center - lowrange)/resolution;
    array_length = 1+num_upper_segments+num_lower_segments;
    
    //Initialize distance and orientation arrays
    initialize_arrays();

    //Default servo response time, to put delay between sensor readings to be sure sensor is pointing in proper direction
    estimated_servo_response_time = 100000; //in us (microseconds)
}

bool SODAR::initialize_arrays()
{
    //Initialize size of distance and orientation arrays
    int distance[array_length];
    int orientation[array_length] ;

    //Initialize distance array to all very large numbers
    for(int i=0; i<array_length; i++)
    {
        distance[i] = 9999; //A large number that won't interfere with our algorithm calculations
    }

    //Initailize orientations array to proper degrees values
    int lower_bound = center - (resolution*num_lower_segments); //
    int upper_bound = center + (resolution*num_upper_segments);
    for(int i=0; i<array_length; i++)
    {
        orientation[i] = lower_bound + (i*resolution);
    }

}

void SODAR::update()
{
    //
}

void SODAR::next()
{

}

void SODAR::start()
{

}