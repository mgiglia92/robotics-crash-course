// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * mpu6050.h -- MPU6050 interface
 *
 * Copyright (C) 2020-2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2020  Andrew Lorber <andrewlorber@aol.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_MPU6050_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_MPU6050_H

// #include <Arduino.h>
// #include <Wire.h>
#include "hardware/i2c.h"

#define RCC_MPU6050_I2C_ADDR 0x68


class MPU6050 {
public:
	uint8_t     i2c_addr = RCC_MPU6050_I2C_ADDR;
	i2c_inst_t* i2c_inst;
	// TwoWire *wire     = &Wire;

	int16_t raw_ax   = 0;
	int16_t raw_ay   = 0;
	int16_t raw_az   = 0;
	int16_t raw_temp = 0;
	int16_t raw_wx   = 0;
	int16_t raw_wy   = 0;
	int16_t raw_wz   = 0;
	float   ax_bias  = 0.0;
	float   ay_bias  = 0.0;
	float   az_bias  = 0.0;
	float   wx_bias  = 0.0;
	float   wy_bias  = 0.0;
	float   wz_bias  = 0.0;

public:
	// void begin(void);
	// void begin(int i2c_addr);
	void begin(i2c_inst_t*);
	void begin_pico();
	// void begin(TwoWire *wire);
	// void begin(int i2c_addr, TwoWire *wire);

	/*
	 * Run this function while the robot is at equilibrium (no
	 * motion).  This will take 100 readings of the sensor, average
	 * them, and then set the zero-error bias for each data point
	 * (except temperature).  Note that the z-axis bias is
	 * incorrect, this is to remove the gravity vector.  This is
	 * fine since we're not currently using the z-axis acceleration
	 * data for any feedback controls.
	 */
	void calibrate(void);

	float getAccel(char axis);
	float getAccelX(void);
	float getAccelY(void);
	float getAccelZ(void);
	float getAngVel(char axis);
	float getAngVelX(void);
	float getAngVelY(void);
	float getAngVelZ(void);
	float getTemp(void);
	void update(void);
	void update_pico(void);
};


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_SENSOR_MPU6050_H */
