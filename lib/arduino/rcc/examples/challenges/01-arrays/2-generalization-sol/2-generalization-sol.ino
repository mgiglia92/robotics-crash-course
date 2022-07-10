/*
 * 2-generalization-sol.ino -- array generalization solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <rcc.h>


int left_motor[]  = {100,    0,  200, -100,    0,   0,   50,  200, -200, -150,  200, -50};
int right_motor[] = {100,  200,    0, -100, -200,   0,  -25,  200, -200,  150, -200, -50};
int delays[]      = {800, 1000, 1000,  800, 2000, 500, 4000, 2000, 2000, 1000, 4000, 800};

int *dance[] = {left_motor, right_motor, delays};


void print_2d_array(int **a, int n, int m)
{
  for (int i = 0; i < n; i++) {
    for (int j = 0; i < m; j++) {
      Serial.print("[");
      Serial.print(i);
      Serial.print(", ");
      Serial.print(j);
      Serial.print("] = ");
      Serial.println(a[i][j]);
    }
  }
}

void setup()
{
  Serial.begin(9600);

  //aka print_2d_array(dance, 3, 12);
  print_2d_array(dance, ARRAYSIZE(dance), ARRAYSIZE(left_motor));
}

void loop()
{
}
