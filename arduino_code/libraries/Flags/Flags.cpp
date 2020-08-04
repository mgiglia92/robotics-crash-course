#include "Flags.h"
#include <Arduino.h>

// Returns flag for specified direction
// 'L' = left, 'C' = center, 'R' = right
bool Flags::is_object(char direction){
    if(direction == 'L'){
        return object_left;
    } else if(direction == 'C'){
        return object_center;
    } else if(direction == 'R'){
        return object_right;
    } else {
        // If incorrect char is given
        return false;
    }
}

// Sets flag for specified direction
// 'L' = left, 'C' = center, 'R' = right
// 'A' = all (used for setting all to "false" if needed)
void Flags::set_object(char direction, bool object){
    if(direction == 'L' || direction == 'A'){
        object_left = object;
    }
    if(direction == 'C' || direction == 'A'){
        object_center = object;
    }
    if(direction == 'R' || direction == 'A'){
        object_right = object;
    }
}
