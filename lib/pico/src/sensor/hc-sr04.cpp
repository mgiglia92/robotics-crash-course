// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * hc-sr04.cpp -- HC-SR04 interface
 *
 * Copyright (C) 2020-2022  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2020  Andrew Lorber <andrewlorber@aol.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include "hc-sr04.h"

#include <Arduino.h>

#include <PinChangeInterrupt.h>


#define PULSE_LEN_US 10


HC_SR04_async *HC_SR04_async::instance = nullptr;


void HC_SR04::begin(void)
{
	pinMode(echo_pin,  INPUT);
	pinMode(trig_pin, OUTPUT);

	// ensure our trigger pin is inactive
	digitalWrite(trig_pin, LOW);
}

void HC_SR04::begin(uint8_t echo_pin, uint8_t trig_pin)
{
	this->echo_pin = echo_pin;
	this->trig_pin = trig_pin;

	begin();
}

unsigned long HC_SR04::pulse(unsigned long timeout_us = RCC_HC_SR04_TIMEOUT_US)
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

static void HC_SR04_async::pulse_isr(void)
{
	if (instance->pulse_done) {
		// temporarily disable our ISR until it is needed again
		disablePinChangeInterrupt(
			digitalPinToPinChangeInterrupt(instance->echo_pin)
		);
		return;
	}

	uint8_t reason = getPinChangeInterruptTrigger(
		digitalPinToPinChangeInterrupt(instance->echo_pin)
	);

	switch (reason) {
		case RISING:
			instance->pulse_us = micros();
			break;

		case FALLING:
			instance->pulse_us   = micros() - instance->pulse_us;
			instance->pulse_done = true;
			break;
	}
}

void HC_SR04_async::begin(void)
{
	if (instance) return;
	instance = this;

	pinMode(echo_pin,  INPUT);
	pinMode(trig_pin, OUTPUT);

	// ensure our trigger pin is inactive
	digitalWrite(trig_pin, LOW);

	attachPinChangeInterrupt(
		digitalPinToPinChangeInterrupt(echo_pin),
		pulse_isr,
		CHANGE
	);

	// temporarily disable our ISR until it is needed
	disablePinChangeInterrupt(digitalPinToPinChangeInterrupt(echo_pin));
}

void HC_SR04_async::begin(uint8_t echo_pin, uint8_t trig_pin)
{
	this->echo_pin = echo_pin;
	this->trig_pin = trig_pin;

	begin();
}

unsigned long HC_SR04_async::getDuration(void)
{
	return (pulse_done && (pulse_us < pulse_timeout_us)) ? pulse_us : 0;
}

bool HC_SR04_async::isDone(void)
{
	noInterrupts();

	if (!pulse_done) {
		if (micros() - pulse_start_us >= pulse_timeout_us) {
			pulse_us   = 0;
			pulse_done = true;
		}
	}

	interrupts();

	if (pulse_done) {
		disablePinChangeInterrupt(
			digitalPinToPinChangeInterrupt(echo_pin)
		);
	}

	return pulse_done;
}

void HC_SR04_async::pulse(unsigned long timeout_us = RCC_HC_SR04_TIMEOUT_US)
{
	pulse_timeout_us = timeout_us;

	digitalWrite(trig_pin, HIGH);
	delayMicroseconds(PULSE_LEN_US);
	digitalWrite(trig_pin, LOW);

	pulse_start_us = micros();
	pulse_done     = false;
	enablePinChangeInterrupt(digitalPinToPinChangeInterrupt(echo_pin));
}
