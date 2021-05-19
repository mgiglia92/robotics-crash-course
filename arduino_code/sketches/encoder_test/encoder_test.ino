
#include "odom.h"
#include <motor_control.h>

Odom odom;

void setup() {
  Serial.begin(115200);
  odom.setup_interrupts();
 
}

void loop() {
//  turn_90_deg();   
  Serial.print(odom.left_count);
  Serial.print(" | ");
  Serial.println(odom.right_count);
}

void turn_90_deg()
{
  if(odom.right_count < 100) { raw_motor_control(0, 150); }
  else { raw_motor_control(0,0); }
}
