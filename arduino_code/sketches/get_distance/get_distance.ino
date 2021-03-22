#include <HC_SR04.h> //Import the Distance sensor library
HC_SR04 sensor(10,2,0); //Summon the distance sensor class variable
double distance; //distance global variable to store distance value from sensor

void setup() {
  distance_sensor_initialization(); //run the initialization procedure for the sensor!
  Serial.begin(9600); //Being serial comms
}

void loop() {

  get_distance(); //update the distance from the distance sensor
  Serial.println(distance); //Print the distance
}

//---------------------------------------------------------------
/* Use this function to get the distance from the distance sensor.
 * The logic to check if the sensor is finished, and restarting the sensor
 * is implemented here so you don't have to worry about it inside your void loop()
 */
double get_distance()
{
  if(sensor.isFinished()) //check if the distance sensor is finished (if so get distance and start next reading)
  {
    distance = sensor.getRange(); //update distance variable to be current distance sensor reading
    sensor.start(); //restart the sensor
  }
  return distance; //return the distance variable (notice that if the sensor is not finished, we just
  //send the distance variable that is unchanged!
}

//----------------------------------------
/* The initialization steps for the distance sensor.
 * Call this in void setup() so that the sensor is ready to be used and you can use the
 * get_distance() function.
 */
void distance_sensor_initialization()
{
  sensor.initialize(); //Begin the sensor (sets the pins, attaches interrupts)
  sensor.start(); //Starts the first reading of the sensor (required so that the loop will work)
}
