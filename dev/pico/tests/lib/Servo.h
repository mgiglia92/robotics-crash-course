
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pwm_util.h"

typedef struct {
    uint gpio;
    uint slice;
    uint chan;
    uint speed;
    uint resolution;
    bool on;
    bool invert;
} Servo;

void ServoInit(Servo *s, uint gpio, bool invert)
{
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    s->gpio = gpio;
    s->slice = pwm_gpio_to_slice_num(gpio);
    s->chan = pwm_gpio_to_channel(gpio);

    pwm_set_enabled(s->slice, false);
    s->on = false;
    s->speed = 0;
    s->resolution = pwm_set_freq_duty(s->slice, s->chan, 50, 0);
    pwm_set_duty(s->slice, s->chan, 250);
    if(s->chan)
    {
        pwm_set_output_polarity(s->slice, false, invert);
    }
    else{
        pwm_set_output_polarity(s->slice, invert, false);
    }
    s->invert = invert;
}


void pwm_set_dutyH(uint slice_num, uint chan, int d)
{
    pwm_set_chan_level(slice_num, chan, pwm_get_wrap(slice_num)*d/10000);
}

void ServoOn(Servo *s)
{
    pwm_set_enabled(s->slice, true);
    s->on = true;
}

void ServoOff(Servo *s)
{
    pwm_set_enabled(s->slice, false);
    s->on = false;
}

void ServoPosition(Servo *s, uint p)
{
    pwm_set_dutyH(s->slice, s->chan, p * 10 + 250);
}

