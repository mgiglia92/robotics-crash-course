/* Arduino Syntax Coding lesson
 * Explains how c++ syntax works for fundamental code writing
 * What are global/local variables?
 * How to create your own function and use that function.
 * Loops/statements: while, for, if, else
 * 
 * NOTE: This code will NOT compile.
 */

//Comments are made by using the // . or multi line comments can use /* */
/*Multi
Line
Comment
*/

//Variable creation, datatypes
int x; //Integer variable named x, not initialized to any value,  will be a random value
int y = 0; //integer y, initialized to 5, integers cannot have decimal points
int yy = -1; //integers can be negative
double z = -1.1526; //double datatype is a numerical data type but it has more storage and can hold decimal points
long zz = -1.23123123; //very similar to double just has more storage
float i = -2.1681465; //very similar to double just more storage
bool isItTrue = true; //Boolean type variable (true/false)

/*NOTE: At the end of each line of code here there is a semi-colon ';' 
 * this lets the compilier know that the line of code has ended and the next line is coming
 * Sometimes there may be sections of code that feel like the end of a line, but they require
 * that there be NO semi-colon, we'll see a couple examples here
 */

//Function creation and calling
/* returnObjectType func_name(datatype arg1, datatype arg2) { //Code that this function executes }
 * 
 *
 */
int add_integers(int a, int b) //note no semi-colon
{//note curly bracket start

  //Create a new "local" variable, important to note that this variable only exists while this function is being executed
  int sum; //We don't need to iniailize to anything just yet
  sum = a + b;

  //Notice that right before the name of this function we have a datatype of int. This is telling the compilier that this function will 
  // return an integer to the space that called it. This is how we return.
  return sum;
  
}//note curly bracket end, also note there is no semi-colon

//How this function would look without all the comments
int add_integers2(int a, int b)
{
  int sum = a + b; //add the two arguemnts
  return sum;
}

//How we can call the above function
int sum = add_integers(4, 5);
//Now sum will have the value 4+5=9

//Types of loops and statements
//While loop
bool condition = true;
while(condition == true) //NOTE: for comparison the syntax is '==' not '='.
{
 //Do something here while condition==true
 add_integers(4,4);
}

//For loop
/* a 'for' loop will run a loop with an interator variable that you define in the ()
 * 1. 'int i = 0;': we must declare the datattype of the iterator and its starting value 'int i = 0;' note the semi-colon
 * 2. 'i<=100;': This is the condition statement, it says run the for loop if this is true, in this case i is less than or equal 100, note the semi-colon
 * 3. The action the iterator takes at the end of each loop. in this case i++ is the same as saying i = i+1, increment by 1, decrement could be done by using i--, 
 *  note no semi-colon here
 */ 
for(int i=0; i<=100; i++)
{
  //Things to do at each iteration
  //For example just print the iterator value
  Serial.println(i);
}

//if else statement
condition = false;
bool truthValue;
if(condition){ truthValue = true;}
else { truthValue = false; }


/* These two functions are special functions. These are the two functions that will hold everything that the arduino will do.
 * The way the arduino upload scheme works is that when the arduino starts up there is a file that calls setup() and then it repeatedly calls the loop() function
 * it might look something like the following:
 * 
 * setup()
 * while(true){ loop(); }
 */
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  add_integers(5, 5);
}
