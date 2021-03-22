#ifndef HC_SR04_H
#define HC_SR04_H

#include <Arduino.h>

class HC_SR04 {
  public:
    // Params are pin numbers & max distance
    HC_SR04(int trigger, int echo, int interrupt, int max_dist=200);
    // Initializes pins
    void initialize();
    // Runs Sonar sensor
    void start();
    bool isFinished(){ return _finished; }
    double getRange();
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
