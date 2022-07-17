/*
 * 2-ultrasonic-sol.ino -- asynchronous ultrasonic timing solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

// Want to see how we implemented HC_SR04_async?
// https://github.com/mgiglia92/robotics-crash-course/blob/main/lib/arduino/rcc/src/sensor/hc-sr04.cpp

#include <PinChangeInterrupt.h>


#define ECHO_PIN 4
#define TRIG_PIN 10

#define PULSE_LEN_US       10
#define HC_SR04_TIMEOUT_US 50000


volatile bool          pulse_done;
volatile unsigned long pulse_us;
unsigned long          pulse_start_us;
unsigned long          pulse_timeout_us;

unsigned long duration_us;


void pulse_isr(void)
{
	if (pulse_done) return;

	uint8_t reason = getPinChangeInterruptTrigger(
		digitalPinToPinChangeInterrupt(ECHO_PIN)
	);

	switch (reason) {
		case RISING:
			pulse_us = micros();
			break;

		case FALLING:
			pulse_us   = micros() - pulse_us;
			pulse_done = true;
			break;
	}
}

void ultrasonicAsyncSetup(void)
{

	pinMode(ECHO_PIN,  INPUT);
	pinMode(TRIG_PIN, OUTPUT);

	// ensure our trigger pin is inactive
	digitalWrite(TRIG_PIN, LOW);

	attachPinChangeInterrupt(
		digitalPinToPinChangeInterrupt(ECHO_PIN),
		pulse_isr,
		CHANGE
	);
}

void ultrasonicAsyncPulse(unsigned long timeout_us = HC_SR04_TIMEOUT_US)
{
	pulse_timeout_us = timeout_us;

	digitalWrite(TRIG_PIN, HIGH);
	delayMicroseconds(PULSE_LEN_US);
	digitalWrite(TRIG_PIN, LOW);

	pulse_start_us = micros();
	pulse_done     = false;
}

unsigned long ultrasonicAsyncPulseDuration(void)
{
  return (pulse_done) ? pulse_us : 0;
}

bool ultrasonicAsyncPulseDone(void)
{
	noInterrupts();

	if (!pulse_done) {
		if (micros() - pulse_start_us > pulse_timeout_us) {
			pulse_us   = 0;
			pulse_done = true;
		}
	}

	interrupts();

	return pulse_done;
}

void setup()
{
  Serial.begin(9600);

  ultrasonicAsyncSetup();

  // send the initial pulse
  ultrasonicAsyncPulse();
}

void loop()
{
  if (ultrasonicAsyncPulseDone()) {
    duration_us = ultrasonicAsyncPulseDuration();

    ultrasonicAsyncPulse();
  }

  Serial.println(duration_us);
}
