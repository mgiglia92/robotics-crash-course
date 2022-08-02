#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H
#include "differentiator.h"
#endif

differentiator::differentiator(float sig, float t_rate){
    sigma = sig;
    ts = t_rate;
    beta = (2.0*sigma - ts) / (2.0*sigma + ts);
    y_dot = 0;
    y_d1 = 0;
}

float differentiator::differentiate(float y){

    // calculate derivative
    y_dot = (beta * y_dot) + (((1 - beta)/ts) * (y - y_d1));

    //set y_d1 to current val
    y_d1 = y;

    //return derivative value
    return y_dot;

}

void differentiator::update_time_parameters(float t, float s){
    ts = t;
    sigma = s;
    beta = (2.0*sigma - ts) / (2.0*sigma + ts);
}

void differentiator::reset(float degrees){
    y_dot = 0;
    y_d1 = degrees;
}