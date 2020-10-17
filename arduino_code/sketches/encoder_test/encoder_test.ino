//#include <robot_config_encoders.h>
#include "robot_config_encoders.h"
#include <motor_control.h>

Config config;

void setup() {
  Serial.begin(115200);
  config.setup_interrupts();
 
}

void loop() {
//  count_r();
  Serial.println(config.right_count);
}
