//Thank you to "jazzycamel" from instructables on this great 'non-blocking' library for the ultrasonic distance sensor
// https://www.instructables.com/id/Non-blocking-Ultrasonic-Sensor-for-Arduino/
//Library name HC_SR04.h/.cpp

#include <Servo.h>
#include <motor_control.h>
#include <HC_SR04.h>
#include <object_detection.h>

// Servo Pin declaration
#define SERVO_PIN 9  //servo connect to D9

//Ultrasonic distance sensor pin declaration
#define TRIG_PIN 10
#define ECHO_PIN 2
#define ECHO_INT 0

//Resolution and limits of searching parameters (for  ultrasonic distance sensor)
#define RESOLUTION 10 //How many degrees to have between detections (this value should be an integer divisior of MAXDEG
#define MAXDEG 130 //Angle of maximum detection
#define MINDEG 50 //Angle of minimum detection

//Global Variables
//Initialize orientations servo will point to
float orientation[((MAXDEG - MINDEG) / RESOLUTION) + 1];
const int array_length = (sizeof(orientation) / sizeof(orientation[0]));
float dist[array_length];
int current_index = 0; // This global variable will be used to iterate through the orientation array to point the sensor in the right direction


Servo head; //Servo Variable
HC_SR04 sensor(TRIG_PIN, ECHO_PIN, ECHO_INT); //Non-blocking ultrasonic distance sensor

//------------------------------------------------------------

//------------------------------------------------------------
//SETUP AND LOOP FUNCTIONS
void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW); //Initialize trigger pin for ultrasonic sensor to LOW
  
  /*init servo*/
  head.attach(SERVO_PIN);
  head.write(90);
  delay(1000);

  //Initialize serial communication
  Serial.begin(9600);

  //Start the ultrasonic distance sensor
  sensor.begin(); // Make sure to run the begin function, this will setup the interrupt pins to make the sensor functional
  sensor.start(); 

  //Initialize orientations array
  for (int i = 0; i < array_length; i++)
  {
    orientation[i] = (RESOLUTION * i) + MINDEG;
  }
  Serial.print(orientation[0]);

}


void loop() {
  //Check if sensor has finished it's last detection run, if so update motor control
  if(sensor.isFinished())
  {
    //Update motor control
    int control_value = discrete_detection(orientation, dist, array_length, 25); //Get control command from detection
    control(control_value, 50); //Control motor with value and power, adjust power so robot moves slowly
    
    //Run detect next, which will rotate servo and start next detection loop
    detect_next();
  }
}
void detect_next()
{
  //Update distance array with value from sensor (we know it's finished because this function is only called when sensor.isFinished() is true
  dist[current_index] = sensor.getRange();
  
  //Use current index to point ultrasonic distance sensor
  head.write(orientation[current_index]);
  
  //Give the sevo a few milliseconds to get to position
  delay(200);

  //Iterate index, if it's too high re-set to 0 index
  current_index++;
  if(current_index == array_length) { current_index = 0; }

  //Restart detection
  sensor.start();
}


//OTHER FUNCTIONS-----------------------------------------------------------------------------------

int analyze_data()
{
  //Analyze dist[] array, and determine where center of object is

  //Simple analysis, pick the minimum and return the angle to point towards
  return find_min(dist, sizeof(dist) / sizeof(dist[0]));

}

//Find the index of the array that has the minimum distance and return
//int find_min(float d[], int arr_size)
//{
//  float minimum = d[0];
//  int min_index = 0;
//  //Start at index 1 so we don't have to have complex logic inside the for loop
//  for (int i = 1; i < arr_size; i++)
//  {
//    if (d[i] < d[min_index]) {
//      min_index = i; //if new value is less than minimum, set min_index to new value index
//      minimum = d[i];
//    }
//  }
//  Serial.print("min index: ");
//  Serial.print(min_index);
//  Serial.print(" | minimum distance value: ");
//  Serial.println(d[min_index]);
//  return min_index;
//}
