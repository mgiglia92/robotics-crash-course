/* Arduino Lesson 3: Classes
 * How to make our own classes to make the code more human readable and navigatable
 */

//Classes have some special syntax to be aware of
/* Classes contain "members and methods". which essentially just means
 * that there are variables and functions connected to the class and only that class
 * We will learn how to create these things and also how to access and use them
 * 
 */
class Robot{ //Class name into open curly brackets
  public: //For now we're going to make everything public to keep things simple and easy.
    int numWheels; //A variable that is part of the Robot class.
    /*
     * The default constructor is below. This line of code tells the class what is required to initialize an instance of
     * this class. It is required for the class to operate. This means when we create a robot class it has to look like this
     * Robot r(5); The type is Robot, the name is 'r', and the numWheels argument is 5
     */
    Robot(int wheelAmt){ numWheels = wheelAmt; } //We set numWheels to the passed in value of wheelsAmt

    void add_wheels(int amt){ numWheels += amt; } //A simple public function to add wheels to the robot
    
  private: //Private variables allow the class to have members and methods that can't be accessed from the outside
            //This is good if we want to have logic that only the class will use on itself, and if there are variables
            //That you do not want people to be able to change while using the class.
            //This is more of an advanced software development tool which is unnecessary for the scope of this class
  int private_method(){return 1;}
  
}; //NOTE: semi-colon after class definition

//Global variable space
//Let's create a robot datatype
Robot r(4);
 
void setup() {
Serial.begin(9600); // To let us do some debugging

}

void loop() {
 Serial.println(r.numWheels);
 r.add_wheels(1);
 //r.private_method();  //Will give us an error saying this function is private
}
