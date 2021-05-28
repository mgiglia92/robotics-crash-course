#ifndef ASYNCTIMER_H
#define ASYNCTIMER_H
#include "AsyncTimer.h"
#endif

#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

// Constructor - Parameter is millis to wait
AsyncTimer::AsyncTimer(double ts0, double ts1, double ts2)
  {
    for(int i=0; i<array_length; i++)
    {
      prev[i]=0;
      run_process[i] = false;
    }
    Ts[0] = ts0;
    Ts[1] = ts1;
    Ts[2] = ts2;
  }

void AsyncTimer::check_dt()
  {
    //Get current time in microseconds
    current = millis();

    //Check delta time for each process
    for(int i=0; i<array_length; i++)
    {
      //If current-prev >= Ts, run_process = true, else false;
      //Remember to convert current-prev to seconds!!!
      if((current-prev[i])/1000.0 >= Ts[i]) 
      { 
        run_process[i] = true; 
        //Remember to reset prev!
        prev[i] = current;
      }
      else { run_process[i] = false; }
    }
  }