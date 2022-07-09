// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * hc-sr04.cpp -- HC-SR04 interface
 *
 * Copyright (C) 2020-2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2020  Andrew Lorber <andrewlorber@aol.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include "hc-sr04.h"

#include <Arduino.h>


HC_SR04 *HC_SR04::_instance(NULL);

HC_SR04::HC_SR04(int trigger, int echo, int interrupt, int max_dist)
    : _trigger(trigger), _echo(echo), _int(interrupt), _max(max_dist), _finished(false)
{
  if(_instance==0) _instance=this;
}

void HC_SR04::initialize(){
  pinMode(_trigger, OUTPUT);
  digitalWrite(_trigger, LOW);
  pinMode(_echo, INPUT);
  attachInterrupt(_int, _echo_isr, CHANGE);
  distance = 99999;
}

void HC_SR04::start(){
  _finished=false;
  digitalWrite(_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigger, LOW);
}

//This function lets you control when to restart the sensor
// in case you want to move the sensor to a new orientation before you start a new reading
double HC_SR04::getRangeReset(){
  //If sensors is finished, update distance value and restart sensor
  if(isFinished())
  {
    distance = ((_end-_start)/(58.0f)); //Convert from time of flight to distance
    start(); //Restart, so we get the next reading ASAP
  }
  //Return last distance value!
  return distance;
}


unsigned int HC_SR04::getRange(){
  return (_end-_start)/(58.0f);
}

void HC_SR04::_echo_isr(){
  HC_SR04* _this=HC_SR04::instance();

  switch(digitalRead(_this->_echo)){
    case HIGH:
      _this->_start=micros();
      break;
    case LOW:
      _this->_end=micros();
      _this->_finished=true;
      break;
  }
}


void ultrasonicSetup(void)
{
	pinMode(RCC_ECHO_PIN,  INPUT);
	pinMode(RCC_TRIG_PIN, OUTPUT);

	// ensure our trigger pin is inactive
	digitalWrite(RCC_TRIG_PIN, LOW);
}
