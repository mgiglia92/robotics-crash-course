// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * hc-sr04.cpp -- HC-SR04 interface
 *
 * Copyright (C) 2020-2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2020  Andrew Lorber <andrewlorber@aol.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include "hc-sr04.h"
#include "hc-sr04_private.h"

#include <Arduino.h>

#include <PinChangeInterrupt.h>


static volatile bool          async_pulse_done;
static          unsigned long async_pulse_timeout_us;
static volatile unsigned long async_pulse_us;


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


void ultrasonicAsyncSetup(void)
{
	ultrasonicSetup();

	attachPinChangeInterrupt(
		digitalPinToPinChangeInterrupt(RCC_ECHO_PIN),
		ultrasonicPulseISR,
		FALLING
	);

	// temporarily disable our ISR until it is needed
	disablePinChangeInterrupt(digitalPinToPinChangeInterrupt(RCC_ECHO_PIN));
}

void ultrasonicAsyncPulse(unsigned long timeout_us = RCC_ULTRASONIC_TIMEOUT_US)
{
	async_pulse_timeout_us = timeout_us;

	digitalWrite(RCC_TRIG_PIN, HIGH);
	delayMicroseconds(PULSE_LEN_US);
	digitalWrite(RCC_TRIG_PIN, LOW);

	async_pulse_done = false;
	async_pulse_us   = micros();
	enablePinChangeInterrupt(digitalPinToPinChangeInterrupt(RCC_ECHO_PIN));
}

bool ultrasonicAsyncPulseDone(void)
{
	noInterrupts();

	if (!async_pulse_done) {
		if (micros() - async_pulse_us > async_pulse_timeout_us) {
			async_pulse_us   = 0;
			async_pulse_done = true;

			disablePinChangeInterrupt(
				digitalPinToPinChangeInterrupt(RCC_ECHO_PIN)
			);
		}
	}

	interrupts();

	return async_pulse_done;
}

inline unsigned long ultrasonicAsyncPulseDuration(void)
{
	return (async_pulse_done) ? async_pulse_us : 0;
}

unsigned long ultrasonicPulse(unsigned long timeout_us = RCC_ULTRASONIC_TIMEOUT_US)
{
#ifdef __AVR_ATmega328P__
	/*
	 * if we're running on the ATmega328P it's worth checking if
	 * interrupts are disabled to to use pulseIn() over
	 * pulseInLong() for more accurate results
	 */
	unsigned long (*pulse)(uint8_t pin, uint8_t state, unsigned long timeout)
		= (SREG & (1 << 7))
		? pulseInLong
		: pulseIn;
#endif /* __AVR_ATmega328P__ */

	digitalWrite(RCC_TRIG_PIN, HIGH);
	delayMicroseconds(PULSE_LEN_US);
	digitalWrite(RCC_TRIG_PIN, LOW);

#ifdef __AVR_ATmega328P__
	return pulse(RCC_ECHO_PIN, HIGH, timeout_us);
#else
	return pulseInLong(RCC_ECHO_PIN, HIGH, timeout_us);
#endif /* __AVR_ATmega328P__ */
}

void ultrasonicSetup(void)
{
	pinMode(RCC_ECHO_PIN,  INPUT);
	pinMode(RCC_TRIG_PIN, OUTPUT);

	// ensure our trigger pin is inactive
	digitalWrite(RCC_TRIG_PIN, LOW);
}


static void ultrasonicPulseISR(void)
{
	if (async_pulse_done) {
		// temporarily disable our ISR until it is needed again
		disablePinChangeInterrupt(
			digitalPinToPinChangeInterrupt(RCC_ECHO_PIN)
		);
		return;
	}

	async_pulse_us   = micros() - async_pulse_us;
	async_pulse_done = true;
}
