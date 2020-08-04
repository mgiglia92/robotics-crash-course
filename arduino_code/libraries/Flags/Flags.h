// Flags Class
// Class to keep track of robot states

// Feel free to add more flags as needed for your algorithm!

#include <Arduino.h>

class Flags{
public:
    // Returns flag for specified direction
    // 'L' = left, 'C' = center, 'R' = right
    bool is_object(char direction);
    // Sets flag for specified direction
    // 'L' = left, 'C' = center, 'R' = right
    // 'A' = all (used for setting all to "false" if needed)
    void set_object(char direction, bool object);
private:
    // Flags to store whether an object is in each direction
    bool object_left = false;
    bool object_center = false;
    bool object_right = false;
};
