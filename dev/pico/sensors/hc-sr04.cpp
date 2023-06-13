// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * hc-sr04.cpp -- HC-SR04 interface
 *
 * Copyright (C) 2020-2022  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2020  Andrew Lorber <andrewlorber@aol.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include "hc-sr04.h"
#include <stdio.h>
#include <iostream>
using namespace std;


#define PULSE_LEN_US 10


HC_SR04_async *HC_SR04_async::instance = nullptr;


// void HC_SR04::begin(void)
// {
// 	// pinMode(echo_pin,  INPUT);
// 	// pinMode(trig_pin, OUTPUT);
// 	gpio_set_function(echo_pin, GPIO_FUNC_SIO);
// 	gpio_set_function(trig_pin, GPIO_FUNC_SIO);
// 	gpio_set_dir(echo_pin, false);
	
// 	// ensure our trigger pin is inactive
// 	gpio_set_irq_enabled_with_callback(echo_pin, GPIO_IRQ_EDGE_RISE, true, &pulse_isr_pico);
// 	gpio_put(trig_pin, false);
// 	// digitalWrite(trig_pin, LOW);
// }

// void HC_SR04::begin(uint8_t echo_pin, uint8_t trig_pin)
// {
// 	this->echo_pin = echo_pin;
// 	this->trig_pin = trig_pin;

// 	begin();
// }

// uint32_t HC_SR04::pulse(uint32_t timeout_us = RCC_HC_SR04_TIMEOUT_US)
// {


// 	// digitalWrite(RCC_TRIG_PIN, HIGH);
// 	// delayMicroseconds(PULSE_LEN_US);
// 	// digitalWrite(RCC_TRIG_PIN, LOW);
// 	gpio_put(RCC_TRIG_PIN, true);
// 	sleep_us(PULSE_LEN_US);
// 	gpio_put(RCC_TRIG_PIN, false);


// 	return pulseInLong(RCC_ECHO_PIN, HIGH, timeout_us);

// }

void HC_SR04_async::pulse_isr_pico(uint gpio, uint32_t events)
{	
	gpio_set_irq_enabled(instance->echo_pin, GPIO_IRQ_EDGE_FALL, false);
	gpio_set_irq_enabled(instance->echo_pin, GPIO_IRQ_EDGE_RISE, false);
	uint32_t now = time_us_32();
	float delta = now - instance->pulse_start_us;
	// printf("Duration, %lu, %f \n", (now - instance->pulse_start_us)*RCC_SOUND_INCHES_PER_MICROSECOND/2);
	cout << "delta: " << delta << " | now: " << now << " | prev: " << instance->pulse_start_us << '\n';
	// cout << "Duration: " << delta << " | " << delta*RCC_SOUND_INCHES_PER_MICROSECOND/2.0 << '\n';
}

void HC_SR04_async::pulse_isr(uint gpio, uint32_t events)
{
	if (instance->pulse_done) {
		// temporarily disable our ISR until it is needed again

		gpio_set_irq_enabled(instance->echo_pin, GPIO_IRQ_EDGE_FALL, false);
		gpio_set_irq_enabled(instance->echo_pin, GPIO_IRQ_EDGE_RISE, false);
		
		return;
	}

	uint8_t reason = events;

	switch (reason) {
		case GPIO_IRQ_EDGE_RISE:
			instance->pulse_us = time_us_32();
			break;

		case GPIO_IRQ_EDGE_FALL:
			instance->pulse_us   = time_us_32() - instance->pulse_us;
			instance->pulse_done = true;
			break;
	}
	
	
}

void HC_SR04_async::begin(void)
{
	if (instance) return;
	instance = this;
	gpio_init(echo_pin);
	gpio_init(trig_pin);
	gpio_set_function(echo_pin, GPIO_FUNC_SIO);
	gpio_set_function(trig_pin, GPIO_FUNC_SIO);
	gpio_set_dir(echo_pin, false);
	gpio_set_dir(trig_pin, true);
	// pinMode(echo_pin,  INPUT);
	// pinMode(trig_pin, OUTPUT);

	// ensure our trigger pin is inactive
	// digitalWrite(trig_pin, LOW);
	gpio_put(trig_pin, 1);
	// gpio_pull_down(echo_pin);

	gpio_set_irq_enabled_with_callback(echo_pin, GPIO_IRQ_EDGE_FALL, true, pulse_isr);
	gpio_set_irq_enabled_with_callback(echo_pin, GPIO_IRQ_EDGE_RISE, true, pulse_isr);
	// attachPinChangeInterrupt(
	// 	digitalPinToPinChangeInterrupt(echo_pin),
	// 	pulse_isr,
	// 	CHANGE
	// );

	// temporarily disable our ISR until it is needed
	gpio_set_irq_enabled(echo_pin, GPIO_IRQ_EDGE_FALL, false);
	gpio_set_irq_enabled(echo_pin, GPIO_IRQ_EDGE_RISE, false);
	// disablePinChangeInterrupt(digitalPinToPinChangeInterrupt(echo_pin));
}

void HC_SR04_async::begin(uint echo_pin, uint trig_pin)
{
	this->echo_pin = echo_pin;
	this->trig_pin = trig_pin;

	begin();
}

uint32_t HC_SR04_async::getDuration(void)
{
	return (pulse_done && (pulse_us < pulse_timeout_us)) ? pulse_us : 0;
}

bool HC_SR04_async::isDone(void)
{
	// noInterrupts();

	// if (!pulse_done) {
	// 	if (micros() - pulse_start_us >= pulse_timeout_us) {
	// 		pulse_us   = 0;
	// 		pulse_done = true;
	// 	}
	// }

	// interrupts();

	// if (pulse_done) {
	// 	disablePinChangeInterrupt(
	// 		digitalPinToPinChangeInterrupt(echo_pin)
	// 	);
	// }

	return pulse_done;
}

void HC_SR04_async::pulse(uint32_t timeout_us = RCC_HC_SR04_TIMEOUT_US)
{
	pulse_timeout_us = timeout_us;

	// digitalWrite(trig_pin, HIGH);
	// delayMicroseconds(PULSE_LEN_US);
	// digitalWrite(trig_pin, LOW);
	gpio_put(trig_pin, 1);
	sleep_us(PULSE_LEN_US);
	gpio_put(trig_pin, 0);

	pulse_start_us = time_us_32();
	pulse_done     = false;
	gpio_set_irq_enabled(echo_pin, GPIO_IRQ_EDGE_FALL, true);
	gpio_set_irq_enabled(echo_pin, GPIO_IRQ_EDGE_RISE, true);
	// enablePinChangeInterrupt(digitalPinToPinChangeInterrupt(echo_pin));
}