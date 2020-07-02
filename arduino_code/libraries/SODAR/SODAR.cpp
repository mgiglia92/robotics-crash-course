#include <SODAR.h>

SODAR::SODAR(int trig_pin,
            int echo_pin,
            int echo_int,
            int servo_pin,
            int cen, 
            int res, 
            int uprange, 
            int lowrange): sensor(trig_pin, echo_pin, echo_int)
{
    center = cen;
    resolution = res;
    upper_range = uprange;
    lower_range = lowrange;
    num_upper_segments = (uprange - center)/resolution;
    num_lower_segments = (center - lowrange)/resolution;
    array_length = 1+num_upper_segments+num_lower_segments;
    actuator_pin = servo_pin;

    //Default servo response time, to put delay between sensor readings to be sure sensor is pointing in proper direction
    estimated_servo_response_time = 500; //in us (microseconds)

    // int distance[array_length];
    // int orientation[array_length] ;
}

bool SODAR::initialize_arrays()
{
    //Initialize size of distance and orientation arrays


    //Initialize distance array to all very large numbers
    for(int i=0; i<array_length; i++)
    {
        distance[i] = 9999; //A large number that won't interfere with our algorithm calculations
    }

    //Initailize orientations array to proper degrees values
    int lower_bound = center - (resolution*num_lower_segments);
    int upper_bound = center + (resolution*num_upper_segments);
    for(int i=0; i<array_length; i++)
    {
        orientation[i] = lower_bound + (i*resolution);
        Serial.print("lower bound: "); Serial.println(lower_bound);
        Serial.print("i: "); Serial.print(i); Serial.print(" ori: "); Serial.println(orientation[i]);
    }
}

void SODAR::update()
{
    //Check if estimate_servo_repsonse_time has elapsed
    // Serial.println(millis() - previous_millis);
    if((millis() - previous_millis) >= estimated_servo_response_time)
    {
        if(!started_flag) { sensor.start(); started_flag=true;}
        if(sensor.isFinished()) //If sensor is finished update value in distance array
        {
            //Update distance value at current index
            distance[current_index] = sensor.getRange();
            //Move to next position
            next();
        }
        else{return;}
    }
}

void SODAR::next()
{
    //Increment/Decrement index (depending on which end you're at)
    if(current_index == array_length-1){down_flag = true;}
    else if(current_index == 0) {down_flag = false;}

    if(down_flag==true) { current_index--;}
    else{current_index++;}

    //write servo to position 
    actuator.Servo::write(orientation[current_index]);

    //Reset previous_millis
    previous_millis = millis();

    //restart sensor started_flag
    started_flag=false;
}

void SODAR::init()
{   //Instantiate Servo class
    actuator.attach(actuator_pin);
    actuator.write(0);
    sensor.begin(); // Initialize pins etc... for sensor
    current_index = 0;  //Initialize index to 0
    previous_millis = 0;
    actuator.Servo::write(orientation[current_index]);  //Move servo to orientation at initial index of 0
    sensor.start();
}