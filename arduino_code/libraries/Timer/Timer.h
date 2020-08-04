// Timer Class
#include <Arduino.h>

class Timer{
public:
    // Constructor - Parameter is millis to wait
    Timer(float num_milli);
    // Starts / restarts the timer
    void start();
    // Checks if timer is finished
    // Parameter defeaulted tp "true". If "false" then user must restart timer with "start()".
    bool is_finished(bool restart = true);
    
private:
    float old_millis;   // Time when timer began
    float wait_time;    // # millis to wait
    bool started = false;       // Whether timer has begun
};
