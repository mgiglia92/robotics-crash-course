
#include <robot_config_encoders.h>

// #include "WInterrupts.c"

Config::Config(){
    volatile int right_count;
    // left_count=0;
}

void Config::count_r(){
    right_count++;
}

void Config::count_l(){
    left_count++;
}

void Config::setup_interrupts(){
    pinMode(RIGHT_ENCODER, INPUT_PULLUP);
    pinMode(LEFT_ENCODER, INPUT);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER), cr, RISING);
    Serial.print("init finished");
}

void cr(){
    Config::count_r();
}