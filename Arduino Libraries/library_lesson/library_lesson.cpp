/*This is the .cpp file. Here we will take the members and methods named in the .h file
and give them implementation by defining how the construtor and functions compute.

NOTE: We will start using something called 'namespaces' here. Each line of code uses a 
namespace. That way variables with identical names that are part of different classes
can be identified by declaring which namespace you want to use.
To do this we simply put the name of the namespace first then two :: before typing the
variable name we want to access. This goes for functions as well.
*/

#include <library_lesson.h>

//Default Constructor note it doesn't have a type before it
Robot::Robot(int amt){
    numWheels = amt; //Set numWheels to argument passed in
}

//Function inside Robot, note that before the name of the function we still have to declare the datatype the function will return
//Void in this case means won't return anything
void Robot::add_wheels(int amt){
    numWheels += amt;

}