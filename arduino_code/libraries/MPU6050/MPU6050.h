//MPU6050 is an accel/gyro/temp sensor that uses I2C to communicate
//This class will allow user to update the sensor state, and grab individual values

#include <Arduino.h>

class MPU6050{
    public:
        MPU6050(int sda, int scl); //Default constructor
        bool update();
        float get_accel(char);
        float get_ang_vel(char);
        float get_temp();
        bool initialize();
        void calibrate();
        void calibrate_request();
        //Addresses
        int MPU6050_addr;
        //Sensitivity parameters
        float gyro_sensitivity;
        float accel_sensitivity;
        //calibration bias parameters
        float _ax_bias;
        float _ay_bias;
        float _az_bias;
        float _wx_bias;
        float _wy_bias;
        float _wz_bias;
        //Accelerations
        float _ax;
        float _ay;
        float _az;
        //Angular velocities
        float _wx;
        float _wy;
        float _wz;
        //Temperature
        float _temp;
};      
