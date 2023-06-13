// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * hc-sr04.h -- HC-SR04 interface
 *
 * Copyright (C) 2020-2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2020  Andrew Lorber <andrewlorber@aol.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_HC_SR04_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_HC_SR04_H

#include "pico/stdlib.h"

#define RCC_ECHO_PIN 4
#define RCC_TRIG_PIN 10

#define RCC_HC_SR04_TIMEOUT_US 5000

#define RCC_SOUND_METERS_PER_MICROSECOND      0.000346
#define RCC_SOUND_CENTIMETERS_PER_MICROSECOND 0.0346
#define RCC_SOUND_FEET_PER_MICROSECOND        0.00113517
#define RCC_SOUND_INCHES_PER_MICROSECOND      0.013622

#define duration2distance(DURATION_US, DISTANCE_PER_US) \
	(DISTANCE_PER_US * (DURATION_US / 2))
#define duration2meters(DURATION_US) \
	duration2distance(DURATION_US, RCC_SOUND_METERS_PER_MICROSECOND)
#define duration2centimeters(DURATION_US) \
	duration2distance(DURATION_US, RCC_SOUND_CENTIMETERS_PER_MICROSECOND)
#define duration2feet(DURATION_US) \
	duration2distance(DURATION_US, RCC_SOUND_FEET_PER_MICROSECOND)
#define duration2inches(DURATION_US) \
	duration2distance(DURATION_US, RCC_SOUND_INCHES_PER_MICROSECOND)


// class HC_SR04 {
// private:
// 	uint8_t echo_pin = RCC_ECHO_PIN;
// 	uint8_t trig_pin = RCC_TRIG_PIN;

// public:
// 	void begin(void);
// 	void begin(uint8_t echo_pin, uint8_t trig_pin);
// 	unsigned long pulse(unsigned long timeout_us = RCC_HC_SR04_TIMEOUT_US);
// };

class HC_SR04_async {
private:
	static HC_SR04_async *instance;

	uint8_t echo_pin = RCC_ECHO_PIN;
	uint8_t trig_pin = RCC_TRIG_PIN;

	volatile bool          pulse_done = false;
	volatile uint32_t pulse_us;
	uint32_t          pulse_start_us;
	uint32_t          pulse_timeout_us = RCC_HC_SR04_TIMEOUT_US;

	volatile uint32_t start;
	volatile uint32_t end;

	static void pulse_isr(uint, uint32_t);
	static void pulse_isr_pico(uint, uint32_t);

public:
	void begin(void);
	void begin(uint echo_pin, uint trig_pin);
	uint32_t getDuration(void);
	bool isDone(void);
	void pulse(uint32_t);
};


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_HC_SR04_H */
