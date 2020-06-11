/* Student Task: 
 * Build the following functions for two integers as input arguments 
 * and returns the appropriate value as the appropriate datatype:
 * 
 * Add 2 integers
 * Subtract 2 integers
 * Multiply 2 integers
 * divide 2 integers (NOTE: Does this function always return an integer? If not, what datatype should it return?)
 */


 void setup()
 {
  //Begin serial communication so we can read the outputs later
  Serial.begin(9600);
  Serial.println("Syntax Task");
 }

void loop()
{
  //Create two integers to send to our functions
  int val_a = 4;
  int val_b = 3;

  //Run add function and print
  int add_return = add(val_a, val_b);
  Serial.print("Add return: ");
  Serial.println(add_return);

  //Run subtract function and print

  //Run multiple function and print

  //Run divide function and print
}

//Place your functions here

//add
int add(int a, int b)
{
  return a+b;
}

//subtract

//multiply

//divide
