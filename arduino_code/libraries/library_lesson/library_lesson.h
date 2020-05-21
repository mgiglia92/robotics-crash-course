/* A library consists of two files, first the 'header' file which has a .h file type
and a .cpp file.
These together allow users to define classes, functions, compiler arguments etc...
We will create a simple class again using the .h and .cpp files then import them into an arduino
.ino file and use them just like we did with arduino_lesson_3 
*/

/*First let's define a class. Important: A header file is supposed to leave the class abstract
We do NOT want to define what exactly the functions do in this file, just their names and types.
We will define what the functions do in the .cpp file
*/
#include <Arduino.h> //Since we'll be using arduino, it's important to make sure the arduino library is imported

class Robot{
    public:
        int numWheels;
        Robot(int); //Default constructor, notice we have 1 argument here, we don't name it we just give the data type
        void add_wheels(int);
    
    //We won't be using any private functions here
};