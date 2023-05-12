// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * odom_impl.h -- encoder interface implementation
 *
 * Copyright (C) 2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Catherine Van West <cat@vanwestco.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_ODOM_IMPL_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_ODOM_IMPL_H


#include <hardware/gpio.h>
#include <hardware/irq.h>
#include <pico/stdlib.h>


template<int int_pin>
volatile long Odom<int_pin>::count = 0;

template<int int_pin>
const uint32_t Odom<int_pin>::isr_events
	= GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE;

template<int int_pin>
Odom<int_pin>::Odom(bool register_isr)
: sign(1) {
	gpio_set_input_enabled(int_pin, true);

	if (register_isr) attach(encoder_isr, isr_events);

	// enable IO IRQs, i guess?
	irq_set_enabled(IO_IRQ_BANK0, true);
}

template <int int_pin>
Odom<int_pin>::Odom()
: Odom<int_pin>(true) { }

template<int int_pin>
void Odom<int_pin>::attach(void (*isr)(void), uint32_t event_mask)
{
	gpio_set_irq_enabled(int_pin, event_mask, true);
	gpio_add_raw_irq_handler(int_pin, isr);
}

template<int int_pin>
void Odom<int_pin>::encoder_isr(void)
{
	if (gpio_get_irq_event_mask(int_pin) & isr_events) {
		gpio_acknowledge_irq(int_pin, isr_events);
		++count;
	}
}

template<int int_pin>
long Odom<int_pin>::getCount(void)
{
	return sign*count;
}

template<int int_pin>
void Odom<int_pin>::setSign(int sign_)
{
	if (sign_ >= 0) sign = 1;
	else sign = -1;
}

template<int int_pin>
void Odom<int_pin>::setZero()
{
	count = 0;
}

template<int int_pin>
void Odom<int_pin>::setZero(long zero)
{
	count -= zero;
}


template<int int_pin, int dir_pin>
const uint32_t Dir_Odom<int_pin, dir_pin>::isr_events
	= GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE;

template<int int_pin, int dir_pin>
Dir_Odom<int_pin, dir_pin>::Dir_Odom()
: Odom<int_pin>(false) {
	gpio_set_input_enabled(dir_pin, true);
	Odom<int_pin>::attach(encoder_isr, isr_events);
}

template<int int_pin, int dir_pin>
void Dir_Odom<int_pin, dir_pin>::encoder_isr()
{
	if (gpio_get_irq_event_mask(int_pin) & isr_events) {
		gpio_acknowledge_irq(int_pin, isr_events);

		const bool pin_1 = gpio_get(int_pin);
		const bool pin_2 = gpio_get(dir_pin);

		if (pin_1 ^ pin_2) --count;
		else ++count;
	}
}

#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_ODOM_IMPL_H */
