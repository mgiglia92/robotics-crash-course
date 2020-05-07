#include <library_lesson.h>

//Global variable space
//Let's create a robot datatype again
Robot r(4);
 
void setup() {
Serial.begin(9600); // To let us do some debugging

}

void loop() {
 Serial.println(r.numWheels);
 r.add_wheels(1);
}
