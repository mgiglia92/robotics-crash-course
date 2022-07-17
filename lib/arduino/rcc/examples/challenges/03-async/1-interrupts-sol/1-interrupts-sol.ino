/*
 * 1-interrupts-sol.ino -- using interrupts solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#define NEUTRAL  0
#define FORWARD  1
#define BACKWARD 2

#define ENCODER_LEFT  2
#define ENCODER_RIGHT 3

#define DELAY_MS 2000


volatile uint8_t left_dir;
volatile uint8_t right_dir;

volatile long left_cnt;
volatile long right_cnt;


void encoder_left_isr(void)
{
  switch (left_dir) {
    case FORWARD:
      ++left_cnt;
      break;

    case BACKWARD:
      --left_cnt;
      break;
  }
}

void encoder_right_isr(void)
{
  switch (right_dir) {
    case FORWARD:
      ++right_cnt;
      break;

    case BACKWARD:
      --right_cnt;
      break;
  }
}

long get_left_cnt(void)
{
  long tmp = left_cnt;

  noInterrupts();
  left_cnt = 0;
  interrupts();

  return tmp;
}

long get_right_cnt(void)
{
  long tmp = right_cnt;

  noInterrupts();
  right_cnt = 0;
  interrupts();

  return tmp;
}

void setup()
{
  Serial.begin(9600);

  pinMode(ENCODER_LEFT,  INPUT);
  pinMode(ENCODER_RIGHT, INPUT);

  attachInterrupt(
      digitalPinToInterrupt(ENCODER_LEFT),
      encoder_left_isr,
      CHANGE
    );

  attachInterrupt(
      digitalPinToInterrupt(ENCODER_RIGHT),
      encoder_right_isr,
      CHANGE
    );
}

void loop()
{
  Serial.println("in neutral for 2s");
  left_dir = right_dir = NEUTRAL;
  delay(DELAY_MS);
  Serial.print("delta: (");
  Serial.print(get_left_cnt());
  Serial.print(", ");
  Serial.print(get_right_cnt());
  Serial.println(")");

  Serial.println("in forward for 2s");
  left_dir = right_dir = FORWARD;
  delay(DELAY_MS);
  Serial.print("delta: (");
  Serial.print(get_left_cnt());
  Serial.print(", ");
  Serial.print(get_right_cnt());
  Serial.println(")");

  Serial.println("in backward for 2s");
  left_dir = right_dir = BACKWARD;
  delay(DELAY_MS);
  Serial.print("delta: (");
  Serial.print(get_left_cnt());
  Serial.print(", ");
  Serial.print(get_right_cnt());
  Serial.println(")");
}
