//MPU6050 is an accel/gyro/temp sensor that uses I2C to communicate
//This class will allow user to update the sensor state, and grab individual values

#include <Arduino.h>

class MPU6050{
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
