#include <motor_control.h>
#include <HC_SR04.h>

//constants that we can use throughout our program
#define FORWARD_POWER 155
#define TURN_POWER 100

//Global variables
HC_SR04 sensor(10, 2, 0);
double distance;
double threshold_forward = 15; //threshold distance from object to trigger end of forward motion
double threshold_turn = 25; // threshold distance to trigger end of turning motion (should be larger than thresh_forward)
//--------------------------------------------------------------------
void setup() {
  // Initialize the distance sensor
  sensor.begin();
  
  // Start the first reading of the sensor
  sensor.start(); 
  
  // Wait till sensor has first distance reading
  while(sensor.isFinished() == false){}
  
  // Sensor has reading, update distance value
  distance = sensor.getRange();
  
  //Start serial comms for debugging
  Serial.begin(9600);
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Main Loop
void loop() {
//----------------------------------------------------------------------
/*Subroutine 1: Drive forward till distance < threshold while updating distance variable
 * 
 */
  while(distance > threshold_forward)
  {
    // Drive forward
    raw_motor_control(FORWARD_POWER, FORWARD_POWER);

    updateDistance();

    debug_print();
  }
  
//----------------------------------------------------------------------
/*Subroutine 2: Rotate slowly counter clockwise until distance variable > threshold + noise compensation amount 
*/
  while(distance < (threshold_turn))
  {
    //set motors to rotate counter clockwise slowly
    raw_motor_control(-1*TURN_POWER, TURN_POWER);

    updateDistance();

    debug_print();
  }

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*Functions: These are used to prevent yourself from having to write the same logic
 * multiple times in the same piece of code. Since we check if the distance sensor is finished
 * multiple times in this loop, it makes the code more readable if we lump that logic into 
 * a custom function.
 */


//updateDistance
/*updateDistance will do all the necessary checks on the distance sensor
 and update the distance variable when capable. If it can't update the 
 distance variable then it will remain unchanged and the algorithm will make the
 same decision it made the prior.
 */
void updateDistance()
{
      if(sensor.isFinished() == true)
    {
      distance = sensor.getRange();

      //Restart distance sensor for a new reading
      sensor.start();
    }
}

//debug_print
/*Print important variables to serial monitor
 * Putting this in a fucntion so i dont need to type
 * out each serial.print() in each while loop
 */
void debug_print()
{
  //Print the distance value to the serial monitor
  Serial.print("dist: "); Serial.println(distance);
}
