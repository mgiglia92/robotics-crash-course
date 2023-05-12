// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * mpu6050.cpp -- MPU6050 interface
 *
 * Copyright (C) 2020-2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2020  Andrew Lorber <andrewlorber@aol.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include "mpu6050.h"

#include <Arduino.h>
#include <Wire.h>


#define ACCEL_SENSITIVITY 16384.0
#define GYRO_SENSITIVITY  131.0


void MPU6050::begin(void)
{
	// power management register
	wire->beginTransmission(i2c_addr);
	wire->write(0x6b);
	wire->write(0x00);
	wire->endTransmission();

	// set gryo sensitivty to +-250
	wire->beginTransmission(i2c_addr);
	wire->write(0x1b);
	wire->write(0x00);
	wire->endTransmission();

	// set acceleration sensitivity to +-2g
	wire->beginTransmission(i2c_addr);
	wire->write(0x1c);
	wire->write(0x00);
	wire->endTransmission();
}

void MPU6050::begin(int i2c_addr)
{
	this->i2c_addr = i2c_addr;

	begin();
}

void MPU6050::begin(TwoWire *wire)
{
	this->wire = wire;

	begin();
}

void MPU6050::begin(int i2c_addr, TwoWire *wire)
{
	this->i2c_addr = i2c_addr;
	this->wire     = wire;

	begin();
}

void MPU6050::calibrate(void)
{
	float ax_sum = 0;
	float ay_sum = 0;
	float az_sum = 0;
	float wx_sum = 0;
	float wy_sum = 0;
	float wz_sum = 0;

	for (int i = 0; i < 100; i++) {
		update();

		ax_sum += raw_ax / ACCEL_SENSITIVITY;
		ay_sum += raw_ay / ACCEL_SENSITIVITY;
		az_sum += raw_az / ACCEL_SENSITIVITY;
		wx_sum += raw_wx / GYRO_SENSITIVITY;
		wy_sum += raw_wy / GYRO_SENSITIVITY;
		wz_sum += raw_wz / GYRO_SENSITIVITY;
	}

	// NOTE: ax_bias will be incorrect since we're removing the
	// gravity components and assuming calibration on a level
	// surface
	ax_bias = ax_sum / 100;
	ay_bias = ay_sum / 100;
	az_bias = az_sum / 100;
	wx_bias = wx_sum / 100;
	wy_bias = wy_sum / 100;
	wz_bias = wz_sum / 100;
}

float MPU6050::getAccel(char axis)
{
	switch (axis) {
		case 'x': return getAccelX();
		case 'y': return getAccelY();
		case 'z': return getAccelZ();
	}

	return NAN;
}

float MPU6050::getAccelX(void)
{
	return (raw_ax / ACCEL_SENSITIVITY) - ax_bias;
}

float MPU6050::getAccelY(void)
{
	return (raw_ay / ACCEL_SENSITIVITY) - ay_bias;
}

float MPU6050::getAccelZ(void)
{
	return (raw_az / ACCEL_SENSITIVITY) - az_bias;
}

float MPU6050::getAngVel(char axis)
{
	switch (axis) {
		case 'x': return getAngVelX();
		case 'y': return getAngVelY();
		case 'z': return getAngVelZ();
	}

	return NAN;
}

float MPU6050::getAngVelX(void)
{
	return (raw_wx / GYRO_SENSITIVITY) - wx_bias;
}

float MPU6050::getAngVelY(void)
{
	return (raw_wy / GYRO_SENSITIVITY) - wy_bias;
}

float MPU6050::getAngVelZ(void)
{
	return (raw_wz / GYRO_SENSITIVITY) - wz_bias;
}

float MPU6050::getTemp(void)
{
	// TODO: figure out the names for these constants
	return (raw_temp / 340) + 36.53;
}

void MPU6050::update(void)
{
	wire->beginTransmission(i2c_addr);
	wire->write(0x3b);
	wire->endTransmission(false);
	wire->requestFrom(i2c_addr, 14);

	raw_ax   = (wire->read() << 8) | wire->read();
	raw_ay   = (wire->read() << 8) | wire->read();
	raw_az   = (wire->read() << 8) | wire->read();
	raw_temp = (wire->read() << 8) | wire->read();
	raw_wx   = (wire->read() << 8) | wire->read();
	raw_wy   = (wire->read() << 8) | wire->read();
	raw_wz   = (wire->read() << 8) | wire->read();
}
