// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * encoder.cpp -- encoder interface
 *
 * Copyright (C) 2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include "encoder.h"

#include <Arduino.h>

#include <PinChangeInterrupt.h>


//Initialize static class members so they are defined for the ISR
static volatile int Odom::right_count=0;
static volatile int Odom::left_count=0;


Odom::Config(){
    // left_count=0;
}


static void Odom::count_r(){
    right_count++;
}

static void Odom::count_l(){
    left_count++;
}

void Odom::setup_interrupts(){
    pinMode(RIGHT_ENCODER, INPUT_PULLUP);
    pinMode(LEFT_ENCODER, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER), cr, RISING);
    attachPCINT(digitalPinToPCINT(LEFT_ENCODER), cl, RISING);
    Serial.print("init finished");
}

void cr(){
    Odom::count_r();
}

void cl(){
    Odom::count_l();
}
