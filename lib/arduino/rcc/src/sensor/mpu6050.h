// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * mpu6050.h -- MPU6050 interface
 *
 * Copyright (C) 2020  Andrew Lorber <andrewlorber@aol.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_SENSOR_MPU6050_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_SENSOR_MPU6050_H


#include <Arduino.h>
#include <Wire.h>


#define RCC_MPU6050_ADDRESS 0x68

#define RCC_MPU6050_ACCEL_SENSITIVITY 16384.0
#define RCC_MPU6050_GYRO_SENSITIVITY  131.0


class MPU6050 {
private:
	int      i2c_addr = RCC_MPU6050_ADDRESS;
	TwoWire *wire     = &Wire;

	float accel_sensitivity = RCC_MPU6050_ACCEL_SENSITIVITY;
	float gyro_sensitivity  = RCC_MPU6050_GYRO_SENSITIVITY;

public:
	void begin(void);
	void begin(int i2c_addr);
	void begin(TwoWire *wire);
	void begin(int i2c_addr, TwoWire *wire);
	void calibrate(void);
	inline float getAccelSensitivity(void);
	inline float getGyroSensitivity(void);
	inline void setAccelSensitivity(float sensitivity);
	inline void setGyroSensitivity(float sensitivity);
	void update(void);
};

/*
class MPU6050 {
    public:
        MPU6050(int sda, int scl);   // Default constructor
        bool initialize();           // Initializes sensor
        void calibrate();            // Calibrates (Do when not in motion)
        bool update();               // Reads all data from sensor
        float get_accel(char);       // Gets acceleration value - choose x,y,z
        float get_ang_vel(char);     // Gets angular velocity value - choose x,y,z
        float get_temp();            // Gets temperature value
        void calibrate_request();
        //Addresses
        int MPU6050_addr;
        //Sensitivity parameters
        float gyro_sensitivity;
        float accel_sensitivity;
        //calibration bias parameters:
        //  These will be used to "calibrate" the sensors raw values
        float _ax_bias;
        float _ay_bias;
        float _az_bias;
        float _wx_bias;
        float _wy_bias;
        float _wz_bias;
        //Accelerations
        int16_t _ax;
        int16_t _ay;
        int16_t _az;
        //Angular velocities
        int16_t _wx;
        int16_t _wy;
        int16_t _wz;
        //Temperature
        int16_t _temp;
};
*/


#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_SENSOR_MPU6050_H */
