// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * mpu6050.ino -- demonstrate the functionality of the MPU6050
 *
 * Copyright (C) 2020-2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <Wire.h>

#include <rcc.h>


MPU6050 imu;


void setup()
{
  Wire.begin();  // initialize the I2C hardware
  imu.begin();   // initialize the MPU6050

  // on initialization the MPU6050 needs to be calibrated, make sure
  // your robot doesn't move during this time!
  imu.calibrate();

  Serial.begin(9600);
}

void loop()
{
  // gets new readings from sensor
  imu.update();

  // get the acceleration on the x-axis
  float accel_x = imu.getAccelX();
  // float accel_x = imu.getAccel('x');

  // get the acceleration on the y-axis
  float accel_y = imu.getAccelY();
  // float accel_y = imu.getAccel('y');

  // although we can get the acceleration on the z-axis, the reading is
  // incorrect as we have dicarded the gravity vector while calibrating
  float accel_z = imu.getAccelZ();
  // float accel_z = imu.getAccel('z');

  // get the angular velocity about the x-axis
  float ang_vel_x = imu.getAngVelX();
  //float ang_vel_x = imu.getAngVel('x');

  // get the angular velocity about the y-axis
  float ang_vel_y = imu.getAngVelY();
  //float ang_vel_y = imu.getAngVel('y');

  // get the angular velocity about the z-axis
  float ang_vel_z = imu.getAngVelZ();
  //float ang_vel_z = imu.getAngVel('z');

  Serial.print("accel (x, y): (");
  Serial.print(accel_x);
  Serial.print(", ");
  Serial.print(accel_y);
  Serial.println(")");
  Serial.print("ang vel (x, y, z): (");
  Serial.print(ang_vel_x);
  Serial.print(", ");
  Serial.print(ang_vel_y);
  Serial.print(", ");
  Serial.print(ang_vel_z);
  Serial.println(")");

  delay(250);
}
