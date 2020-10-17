// Circle Class
class Circle {
   // Private variables that can only be accessed within the class
   // Place private variables here.
   double radius;
   double pi = 3.14;
   
  // Everything after "public" can be accesed outside of the class (i.e in "setup" or "loop")
  public:
    // Put a constructor here: What information is needed?
    Circle(double);
    // Create a function that returns area (use 3.14 for pi)
    double area(){return (radius*pi*radius);};
    // Create a function that allows the user to update the values
    void set_radius(double rad) {radius = rad;}
};

// Definitions of functions declared above
Circle::Circle(double r){radius = r;}


void setup() {
  // Instantiates the Circle class
  Circle c(4.3);
  
  // Can only print after serial.begin()
  Serial.begin(9600);
  
  // Calls the area() function within the class
  double a = c.area();
  
  // Prints area
  Serial.println(a);
  
  // Updates values
  c.set_radius(5.346);
  
  // Prints new areas
  Serial.println(c.area());
}

void loop() {
  // put your main code here, to run repeatedly:
}
