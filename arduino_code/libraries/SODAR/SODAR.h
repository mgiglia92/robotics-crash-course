#include <Arduino.h>
#include <Servo.h>
#include <HC_SR04.h>

class SODAR
{
    public:
        // Constructor: Takes Servo and HC_SR04 sensor
        SODAR(int, int, int, int, int, int, int, int);
        // Sets array lengths and initialize values to prevent erroneous startup data
        bool initialize_arrays();
        // Starts the scanning process
        void init();
        // Stops the scanning process
        void stop();
        // Checks if current scanning position is finished and can move to the next.
        void update();
        // Called in update(), determines next servo position and sends servo to that position.
        void next();
        // Allows user to reset center, resolution and range from what was initially sent in the constructor.
        void set_center_resolution_and_range(int, int, int, int);
    
        HC_SR04 sensor; // Ultrasonic distance sensor class
        Servo actuator; // Actuator to rotate sensor

        double get_forward_distance();
    
        // Servo orientation array: each element is the degrees value we will turn the servo to in order to sense the distance of an object in front.
        // Defaults the size to 180 length, that way we dont have dynamic allocation issues. Only the first "array_length" of values will be used.
        int orientation[180];
        // Array of distances from HC_SR04, each element is correlated to an element in orientation[] to give us an mapping of distances from the robot at different angles.
        int distance[180];
        // Length of the distance and orientation array
        int array_length;
        //Angle that defines the center of the range (the forward facing direction)
        int center;
        
        int resolution;   // Discretization resolution (in deg) for scanning range
        int upper_range;  // Upper servo angle limit for scanning
        int lower_range;  // Lower servo angle limit for scanning
        // Timing variable to not have to use delay() to enable estimated_servo_response_time delay through millis();
        long previous_millis;
        // Estimated delay time to wait for servo to reach written position
        long estimated_servo_response_time;
        // Number of discretizations elements below center for sensor's range
        int num_upper_segments;
        // Numer of discretization elements above center for sensors' range
        int num_lower_segments;
        int current_index; // The current array index the SODAR sensor is at
        int actuator_pin;
        bool down_flag=false; // Flag for increment or decrement of current_index
        bool started_flag = false; //Flag to see if HC_SR04 has been started to not restart sensor by accident
};
