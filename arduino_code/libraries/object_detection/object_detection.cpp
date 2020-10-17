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
    int control = check_distance(o, dist, len, thresh);
    return control;

}

//Check to make sure that all distances in range are > thresh
int check_distance(float o[], float dist[], int len, int thresh)
{
    //Separate array indexs into left, center, right
    int center_index = int(len/2); //casting to int will truncate remainder if value is odd
    int center_window = 1; //How many indicies around center_index will we count as "center"

    //Get the minimum distance value and index
    int min_index = find_min(dist, len);
    int min_value = dist[min_index];

    //Check if minimum above threshold, if so return 0 to move foward
    if(min_value > thresh) { return 0; }

    //If minimum is within threshold, determine if it exists in right, center, or left, return appropriate 
    //control direction
    //Right segment
    if(min_index < center_index-center_window){ return 3; } //Turn left

    //Center segment
    if(min_index < center_index+center_window && min_index > center_index-center_window)
    {
        return 1; //Move backwards
    }

    //Left segment
    if(min_index < len && min_index > center_index+center_window) { return 2; }//Turn right

}

int find_min(float dist[], int arr_size)
{
  float minimum = dist[0];
  int min_index = 0;
  //Start at index 1 so we don't have to have complex logic inside the for loop
  for (int i = 1; i < arr_size; i++)
  {
    if (dist[i] < dist[min_index]) {
      min_index = i; //if new value is less than minimum, set min_index to new value index
      minimum = dist[i];
    }
  }
  return min_index;
}