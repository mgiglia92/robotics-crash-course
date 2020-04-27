#include <object_detection.h>

//Determine where objects are and return an integer representing the motor control
// function to run
// o - orientation array
//dist - distance array
//len -length of arrays
//thresh - distance threshold to consider object "detected"
int discrete_detection(float o[], float dist[], int len, int thresh)
{
    // Check if we're clear of objects
    bool control = check_clear(o, dist, len, thresh);

    //Return 0 if clear (0 is move forward)
    if(control){ return 0;}

    //Return 1 if not clear (1 is move backward)
    if(!control){ return 1;}
}

//Check to make sure that all distances in range are > thresh
bool check_clear(float o[], float dist[], int len, int thresh)
{
    //Check each element of distance array
    for(int i=0; i < len; i++)
    {
        if(dist[i] <= thresh){ return false; } //If distance is <= thersh, object is detected
        else{ continue; }
    }
    // If we get here no objects have been detected, so return True for 'clear' status
    return true;
}