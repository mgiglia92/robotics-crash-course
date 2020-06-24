#include <motor_control.h>
#include <HC_SR04.h>
#include <SODAR.h>

// Servo Pin declaration
#define SERVO_PIN 9  //servo connect to D9

//Ultrasonic distance sensor pin declaration
#define TRIG_PIN 10
#define ECHO_PIN 2
#define ECHO_INT 0
#define CENTER 91
#define RESOLUTION 10
#define UPPER_RANGE 111
#define LOWER_RANGE 71

//Global Variable
SODAR mysodar(TRIG_PIN, ECHO_PIN, ECHO_INT, SERVO_PIN, 91, 10, 131, 51);


void setup()
{
//  mysodar.init();
  Serial.begin(9600);
  pinMode(SERVO_PIN, OUTPUT);
  mysodar.initialize_arrays();
  mysodar.init();
}

void loop()
{
  mysodar.update();
  print_array(mysodar.distance, mysodar.array_length);
}

void print_array(int arr[], int len)
{
  for (int i = 0; i < len; i++)
  {
    Serial.print(arr[i]);
    Serial.print(" | ");
  }
  Serial.println();
}
