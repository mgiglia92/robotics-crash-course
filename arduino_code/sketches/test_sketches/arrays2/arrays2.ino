//Important array parameters
#define LENGTH 10
#define RESOLUTION 10 //How many degrees to have between detections 
// (this value should be an integer divisior of (MAXDEG - MINDEG)*/
#define MAXDEG 180 //Angle of maximum detection
#define MINDEG 10 //Angle of minimum detection

//Global Variables
// Let's setup 2 arrays that are connected to each other
// First one is called degrees
// Second is called distances
// Both of these will have a length dependent on the #define values above
// This is an important way to define arrays, as we can easily change
// one of the #define parameters and the code will still work, but now with
// different array lengths, etc...

// Let's get the array length by doing the following
// We are taking our Max and Min range defined above and going to divide it by resolution
const int array_length = ((MAXDEG - MINDEG) / RESOLUTION) + 1;
int deg[array_length];
int dist[array_length];


//This function will initialize the array passed to it (you should only pass
// the degrees array to this function)
// It will fill the array with the proper degrees value at each position in the array
// Note that when we pass the first argument into this function (an array argument)
// we are using something calleed pass by reference. Pass by reference isn't passing the data in deg, it's passing
// a pointer to the first cell of data in 'deg'
// Technically speaking the value of deg is actually a pointer pointing to an address in memory that holds data
// When we call deg[integer] we are accessing the data at the pointer correlated to 'number' and returning that value
// So if 'number' is 1, we are accessing the pointer at 1, which is the pointer at 0 iterated forward 1 time
// If we access deg[10] but 'deg' only has a length of 10, we are actually acessing the 11th pointer
// which is outside of the memory allocated for 'deg', and therefore we will be getting 'random' or unknown data
// and if we write to a pointer outside of the array, this can have very bad unintended consequences, such as breaking
// your code completely. So be very careful that you never call or change the value of a pointer that is outside
// of the length of the array
void fill_degrees_array(int deg[], int len)
{
  // For each index within the length of deg[] we will define the integer that is that amount of degrees
  for (int i = 0; i < len; i++)
  {
    deg[i] = (RESOLUTION * i) + MINDEG; // at each index of the array, place the degrees value which will be
    // (cell_position * resolution for each cell) + starting_value
  }

}

//Let's make a function that will fill the array with random numbers
void fill_distance_array(int arr[], int len)
{
  //make a for loop that will parse through the entire array and give each
  // cell a random value
  long r;
  for (int i = 0; i < len; i++)
  {
    //Generate random number
    randomSeed(analogRead(0)); //Use this to truly randomize the arduino (this will shuffle the random() function with the value at analog0.
                   //since analog0 is unconnected it will be noise, and therefore "random"
    r = random(0, 1000);
    //Set the value at the current index to the random value
    arr[i] = int(r); //cast the long to an integer for the array
  }
}

//A function that will print out the array nicely for us to read and debug
void print_array(int arr[], int len)
{
  //Run through the for loop and print each cell with a space next to it
  for (int i = 0; i < len; i++) {
    Serial.print(" | ");
    Serial.print(arr[i]);
  }
  Serial.println();
}

//This function will search through the array passed in as argument 1
// and search for the index that correlates to the minimum value
// it will return this index value for us to use later
// Can we think of some ways that this function will not work optimally?
int find_min(int d[], int len)
{
  int minimum = d[0]; // Assume the first cell is the minimum to start the algorithmic loop
  int min_index = 0; // The index associated with where the minimum value it, we set this to 0 for now for the same reasons as above
  
  //Start at index 1 since we've already did the "processing" for cell 0
  [1, 1, 2, 3, 4, 0]
  for (int i = 1; i < len; i++)
  {
    if (d[i] < d[min_index]) { // Is the new value at the current index less than the current minimum value?
      min_index = i; //if new value is less than minimum, set min_index to new value index
      minimum = d[i]; // Set the minimum value to the value at the minimum index (not used, but could be useful in the future)
    }
  }
  // Some printing for sanity checking
  Serial.print("min index: ");
  Serial.print(min_index);
  Serial.print(" | minimum distance value: ");
  Serial.println(d[min_index]);
  return min_index; // Return the minimum index
}

void setup() {
  // Setup Serial communication
  Serial.begin(9600);
  // Initialize the degrees array
  fill_degrees_array(deg, array_length);
  // Initialize the positions array with random values
  fill_distance_array(dist, array_length);
  // Find the index correlated to the minimum value in the distacnesarray
  int min_index; //Define a variable for minimum index
  min_index = find_min(dist, array_length);
  // Print out the index of the minimum, and the degrees at this index, and the distance at this index
  Serial.print("Index: "); Serial.println(min_index);
  Serial.print("Degrees: "); Serial.println(deg[min_index]);
  Serial.print("Distance: "); Serial.println(dist[min_index]);
  Serial.print("Deg array: "); print_array(deg, array_length);
  Serial.print("Dist array: "); print_array(dist, array_length);
}

void loop() {
  //Do nothing in loop so we can read what was printed to the screen

}
