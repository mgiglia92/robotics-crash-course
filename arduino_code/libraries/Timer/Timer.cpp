#include "Timer.h"
#include <Arduino.h>

// Constructor - Parameter is millis to wait
Timer::Timer(float num_milli): wait_time(num_milli), started(false){}

// Starts timer
void Timer::start(){
    started = true;
    old_millis = millis();
}

// Checks if timer is finished & restarts timer if "true"
bool Timer::is_finished(bool restart = true){
    if(started && millis() - old_millis > wait_time){
        started = false;
        if(restart){ start(); }
        return true;
    } else {
        return false;
    }
}
