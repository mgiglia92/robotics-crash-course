// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * differentiator.cpp -- differentiator
 *
 * Copyright (C) 2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */
#include "differentiator.h"


Differentiator::Differentiator(float sig, float t_rate){
    sigma = sig;
    ts = t_rate;
    beta = (2.0*sigma - ts) / (2.0*sigma + ts);
    y_dot = 0;
    y_d1 = 0;
}

float Differentiator::differentiate(float y){

    // calculate derivative
    y_dot = (beta * y_dot) + (((1 - beta)/ts) * (y - y_d1));

    //set y_d1 to current val
    y_d1 = y;

    //return derivative value
    return y_dot;

}

void Differentiator::update_time_parameters(float t, float s){
    ts = t;
    sigma = s;
    beta = (2.0*sigma - ts) / (2.0*sigma + ts);
}

void Differentiator::reset(float degrees){
    y_dot = 0;
    y_d1 = degrees;
}
