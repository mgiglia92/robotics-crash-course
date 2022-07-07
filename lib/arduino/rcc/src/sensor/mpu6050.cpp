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

		ax_sum += raw_ax / accel_sensitivity;
		ay_sum += raw_ay / accel_sensitivity;
		az_sum += raw_az / accel_sensitivity;
		wx_sum += raw_wx / gyro_sensitivity;
		wy_sum += raw_wy / gyro_sensitivity;
		wz_sum += raw_wz / gyro_sensitivity;
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

inline float MPU6050::getAccelSensitivity(void)
{
	return accel_sensitivity;
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

inline float MPU6050::getAccelX(void)
{
	return (raw_ax / accel_sensitivity) - ax_bias;
}

inline float MPU6050::getAccelY(void)
{
	return (raw_ay / accel_sensitivity) - ay_bias;
}

inline float MPU6050::getAccelZ(void)
{
	return (raw_az / accel_sensitivity) - az_bias;
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

inline float MPU6050::getAngVelX(void)
{
	return (raw_wx / gyro_sensitivity) - wx_bias;
}

inline float MPU6050::getAngVelY(void)
{
	return (raw_wy / gyro_sensitivity) - wy_bias;
}

inline float MPU6050::getAngVelZ(void)
{
	return (raw_wz / gyro_sensitivity) - wz_bias;
}

inline float MPU6050::getGyroSensitivity(void)
{
	return gyro_sensitivity;
}

inline float MPU6050::getTemp(void)
{
	return (raw_temp / 340) + 36.53;
}

inline void MPU6050::setAccelSensitivity(float sensitivity)
{
	accel_sensitivity = sensitivity;
}

inline void MPU6050::setGyroSensitivity(float sensitivity)
{
	gyro_sensitivity = sensitivity;
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

/*
//Default constructor
MPU6050::MPU6050(int sda, int scl)
{
    //Initialize address and parameter values
    gyro_sensitivity = 131.0;
    accel_sensitivity = 16384.0;
    _ax_bias=0;
    _ay_bias=0;
    _az_bias=0;
    _wx_bias=0;
    _wy_bias=0;
    _wz_bias=0;
}

//Initialize the MPU6050 with some standard parameters
bool MPU6050::initialize()
{
    //Power management register
    Wire.begin();
    Wire.beginTransmission(0x68);
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission();

    //configure gyro
    Wire.beginTransmission(0x68); //Address of the sensor on i2c line
    Wire.write(0x1B); //Address we want to edit
    Wire.write(0b0000000); //Write Gyro sensitivity to +-250
    Wire.endTransmission();//end transmission

    //configure accel
    Wire.beginTransmission(0x68);
    Wire.write(0x1C);  //Address we want to edit
    Wire.write(0b00000000); //Write accel sensitivity to +-2g
    Wire.endTransmission();
    delay(100);
    return true;
}

//Read all data from sensor, return true if it completes

bool MPU6050::update()
{
    Wire.beginTransmission(0x68);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    // delay(10);
    Wire.requestFrom(0x68,14,true);
    // Serial.print("bytes aval: "); Serial.println(Wire.available());
    //Data for each axis is stored in two 8 bit registers, bitshift first read by 8 and bitwire OR the next read
    _ax = Wire.read()<<8|Wire.read();
    _ay = Wire.read()<<8|Wire.read();
    _az = Wire.read()<<8|Wire.read();
    _temp = Wire.read()<<8|Wire.read();
    _wx = Wire.read()<<8|Wire.read();
    _wy = Wire.read()<<8|Wire.read();
    _wz = Wire.read()<<8|Wire.read();
    return true;
}

//Get acceleration value, choose x,y,z
float MPU6050::get_accel(char c)
{
    if(c=='x'){ return (_ax/accel_sensitivity) - _ax_bias;}
    if(c=='y'){ return (_ay/accel_sensitivity) - _ay_bias;}
    if(c=='z'){ return (_az/accel_sensitivity) - _az_bias;}
    else { return -999999; }
}

//Get angular velocity value, choost x,y,z
float MPU6050::get_ang_vel(char c)
{
    if(c=='x'){ return (_wx/gyro_sensitivity) - _wx_bias;}
    if(c=='y'){ return (_wy/gyro_sensitivity) - _wy_bias;}
    if(c=='z'){ return (_wz/gyro_sensitivity) - _wz_bias;}
    else{ return -999999; }
}

//Get temperature value
float MPU6050::get_temp() { return (_temp/340) + 36.53; }

//Run this function while the robot is at equilibrium (no motion).
//This will take 100 readings of the sensor, average them, and set the
//zero-error bias for each data (except temperature)
//NOTE: The z axis bias is incorrect, this is removing the gravity vector
//This is okay because we are not currently using the z-axis acceleration data for any feedback controls
void MPU6050::calibrate()
{
    //Sum buffer variables, init to 0
    float ax_sum=0;
    float ay_sum=0;
    float az_sum=0;
    float wx_sum=0;
    float wy_sum=0;
    float wz_sum=0;
    //Get 100 readings from sensor, and average them.
    for(int i=0; i<100; i++)
    {
        //Update sensor and then add each value to sum buffer variables
        update();
        ax_sum += get_accel('x');
        ay_sum += get_accel('y');
        az_sum += get_accel('z');
        wx_sum += get_ang_vel('x');
        wy_sum += get_ang_vel('y');
        wz_sum += get_ang_vel('z');
    }
    //Divide sum values by 100 to get the avg and set bias values
    _ax_bias = ax_sum/100;
    _ay_bias = ay_sum/100;
    _az_bias = az_sum/100; //Z-axis bias will be incorrect since we're removing the gravity components and assuming calibration on a level surface
    _wx_bias = wx_sum/100;
    _wy_bias = wy_sum/100;
    _wz_bias = wz_sum/100;

}
*/
