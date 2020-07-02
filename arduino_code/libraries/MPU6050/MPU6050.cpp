#include "MPU6050.h"
#include <Arduino.h>
#include <Wire.h>

#define MPU6050_ADDRESS 0x68

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
float MPU6050::get_temp() { return _temp; }

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
