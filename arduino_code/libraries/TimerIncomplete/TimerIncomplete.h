// Timer Class
#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

class TimerIncomplete
{
public:
  unsigned long prev; // previous time (in milli or microseconds) that each of the processes we're executed
  unsigned long current; //current time in in milli or microseconds
  double Ts; //The time between process executions for each process (in seconds!!!)
  bool run_process; //An array letting the outside function know which process should be run.

  //---------------------------------
  //Constructor, takes 1 variable, the time between process executions
  AsyncTimer(double);
  
  //-----------------------------------------
  //Check which processes timers are up. Flip run_process value depending if duration has passed.
  void check_dt();
};
