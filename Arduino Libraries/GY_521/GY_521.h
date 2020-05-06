//GY_521 is an accel/gyro/temp sensor that uses I2C to communicate
//This class will allow user to update the sensor state, and grab individual values

#include <Arduino.h>

class GY_521{
    public:
        GY_521(int sda, int scl); //Default constructor
        bool update();
        float get_accel(char);
        float get_ang_vel(char);
        float get_temp();
    
    private:
        //Accelerations
        float _ax;
        float _ay;
        float _az;
        //Angular velocities
        float _wx;
        float _wy;
        float _wz;
        //Temperature
        float temp;
};