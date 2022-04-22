//TimerIncomplete test
#include <TimerIncomplete.h>

double delta_time = 1000; //Time between process executions in milliseconds

//Construct a TimerIncomplete class
TimerIncomplete timer(delta_time);

void setup()
{
  //Start serial comms
  Serial.begin(9600);
}

void loop()
{
  //Have timer check the duration between process executions, and update internal variable
  timer.check_dt();

  //Check the run_process internal variable
  if (timer.run_process == true)
  {
    Serial.print("Process executed!");
    Serial.print(" | ");
    Serial.print(" Current Time: ");
    Serial.print(millis());
    Serial.println();
  }
}
