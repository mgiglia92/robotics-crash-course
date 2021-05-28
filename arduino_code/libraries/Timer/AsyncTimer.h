// Timer Class
#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

class AsyncTimer
{
public:
  static const int array_length=3; //# of processes we're dealing with
  unsigned long prev[array_length]; // previous time (in milli or microseconds) that each of the processes we're executed
  unsigned long current; //current time in in milli or microseconds
  double Ts[array_length]; //The time between process executions for each process (in seconds!!!)
  bool run_process[array_length]; //An array letting the outside function know which process should be run.

  //---------------------------------
  //Constructor
  AsyncTimer(double ts0, double ts1, double ts2);
  
  //-----------------------------------------
  void check_dt();
};
