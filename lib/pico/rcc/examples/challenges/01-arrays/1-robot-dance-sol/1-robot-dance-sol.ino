/*
 * 1-robot-dance-sol.ino -- robot dance solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <rcc.h>


// dance[0] ==> left motor pwm
// dance[1] ==> right motor pwm
// dance[2] ==> millisecond delays
int dance[3][12] = {
  {100,    0,  200, -100,    0,   0,   50,  200, -200, -150,  200, -50},
  {100,  200,    0, -100, -200,   0,  -25,  200, -200,  150, -200, -50},
  {800, 1000, 1000,  800, 2000, 500, 4000, 2000, 2000, 1000, 4000, 800},
};


void setup()
{
  motorSetup();
}

void loop()
{
  // go through the array from the outside in
  for (int i = 0; i < 6; i++) {
    rawMotorCtrl(dance[0][11 - i], dance[1][11 - i]);
    delay(dance[2][11 - i]);
    rawMotorCtrl(dance[0][i], dance[1][i]);
    delay(dance[2][i]);
  }
}
