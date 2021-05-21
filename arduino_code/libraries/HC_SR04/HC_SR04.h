
#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

class HC_SR04 {
  public:
    // Params are pin numbers & max distance
    HC_SR04(int trigger, int echo, int interrupt, int max_dist=200);
    // Initializes pins
    void initialize();
    // Runs Sonar sensor
    void start();
    bool isFinished(){ return _finished; }
    double getRangeReset(); //Update distance member, start next reading if updated, if not return old value
    unsigned int getRange(); //Return distance calculated directly. Need to check if finished before calling this fucntion
    static HC_SR04* instance(){ return _instance; }
    
  private:
    static void _echo_isr();
    double distance;
    int _trigger, _echo, _int, _max;
    volatile unsigned long _start, _end;
    volatile bool _finished;
    static HC_SR04* _instance;
};

#endif
