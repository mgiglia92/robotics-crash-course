#ifndef TIMERINCOMPLETE_H
#define TIMERINCOMPLETE_H
#include "TimerIncomplete.h"
#endif

#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

// Constructor - Parameter is millis to wait between process executions
TimerIncomplete::TimerIncomplete(double ts0)
  {

    //Initialize the timing variables
    current=?
    prev=?
    run_process = ?
    
    Ts[0] = ?
  }

void TimerIncomplete::check_dt()
  {
    //Update current time in microseconds to the 'current' variable
    current = ?

    //Check delta time from now to the previous execution time
    if()
    {
      //Set the run process variable to true

      //Update the 'prev' variable so it is updated to the time this process was executed
      
    }

    else 
    {
      //Make sure run_process is set to false
      run_process = false;
    }

  }