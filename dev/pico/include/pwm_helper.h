#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <math.h>
#include <stdlib.h>
#include "rcc_pins.h"

using namespace std;

typedef struct 
{
    uint gpio;
    uint slice;
    uint chan;
    uint speed;
    uint resolution;
    bool on;
    bool invert;
} Servo;

typedef struct
{
    Servo left; //ENA pin
    Servo right; //ENB pin
    uint in1; //left motor dir pins
    uint in2;
    uint in3; //right motor dir pins
    uint in4;
} Motor;

uint32_t pwm_set_freq_duty(uint slice_num, uint chan, uint32_t f, int d)
{
    uint32_t clock = 125000000;
    uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);
    if (divider16 / 16 == 0)
    {
        divider16 = 16;
    }
    uint32_t wrap = clock* 16 / divider16 / f - 1;
    pwm_set_clkdiv_int_frac(slice_num, divider16/16, divider16 & 0xF);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, chan, wrap * d / 100);
    return wrap;
}

uint32_t pwm_get_wrap(uint slice_num)
{
    valid_params_if(PWM, slice_num >= 0 && slice_num < NUM_PWM_SLICES);
    return pwm_hw->slice[slice_num].top;
}

void pwm_set_duty(uint gpio, int d)
{
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    uint chan = pwm_gpio_to_channel(gpio);
    pwm_set_chan_level(slice_num, chan, pwm_get_wrap(slice_num)*d/100);
}

void pwm_set_dutyH(uint slice_num, uint chan, int d)
{
    pwm_set_chan_level(slice_num, chan, pwm_get_wrap(slice_num) * d / 10000);
}

void pwm_set_dutyF(uint slice_num, uint chan, float d)
{
    int frac = static_cast<int>(d*10000);
    frac = min(max(0,frac), 10000);
    pwm_set_chan_level(slice_num, chan, pwm_get_wrap(slice_num)*frac/10000);
}

void ServoInit(Servo *s, uint gpio, bool invert, uint freq)
{
    gpio_init(gpio);
    gpio_set_dir(gpio, true);
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    s->gpio = gpio;
    s->slice = pwm_gpio_to_slice_num(gpio);
    s->chan = pwm_gpio_to_channel(gpio);

    pwm_set_enabled(s->slice, false);
    s->on = false;
    s->speed = 0;
    s->resolution = pwm_set_freq_duty(s->slice, s->chan, freq, 0);
    pwm_set_dutyH(s->slice, s->chan, 0);
    if(s->chan)
    {
        pwm_set_output_polarity(s->slice, false, invert);
    }
    else
    {
        pwm_set_output_polarity(s->slice, invert, false);

    }
    s->invert = invert;
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
    pwm_set_dutyH(s->slice, s->chan, p*10 + 250);
}

void MotorsOn(Motor *m)
{
    pwm_set_enabled(m->left.slice, true);
    pwm_set_enabled(m->right.slice, true);
    m->left.on = true;
    m->right.on = true;
}

void MotorsOff(Motor *m)
{

    pwm_set_enabled(m->left.slice, false);
    pwm_set_enabled(m->right.slice, false);
    m->left.on = false;
    m->right.on = false;

    gpio_put(m->in1, false);
    gpio_put(m->in2, false);
    gpio_put(m->in3, false);
    gpio_put(m->in4, false);
}

void MotorInitGPIO(uint gpio)
{
    gpio_init(gpio);
    gpio_set_dir(gpio, true);
}

void MotorInit(Motor *m, uint gpio_l, uint gpio_r, uint freq)
{
    // ServoInit(&s->left, RCC_ENA, false, freq);
    // ServoInit(&s->right, RCC_ENB, false, freq);
    MotorInitGPIO(gpio_l);
    MotorInitGPIO(gpio_r);
    gpio_set_function(gpio_l, GPIO_FUNC_PWM);
    gpio_set_function(gpio_r, GPIO_FUNC_PWM);
    m->left.slice = pwm_gpio_to_slice_num(gpio_l);
    m->right.slice = pwm_gpio_to_slice_num(gpio_r);
    m->left.chan = pwm_gpio_to_channel(gpio_l);
    m->right.chan = pwm_gpio_to_channel(gpio_r);
    m->left.gpio = gpio_l;
    m->right.gpio = gpio_r;
    m->left.resolution = pwm_set_freq_duty(m->left.slice, m->left.chan, freq, 0);
    m->right.resolution = pwm_set_freq_duty(m->right.slice, m->right.chan, freq, 0);

    MotorInitGPIO(RCC_IN1);
    MotorInitGPIO(RCC_IN2);
    MotorInitGPIO(RCC_IN3);
    MotorInitGPIO(RCC_IN4);
    m->in1 = RCC_IN1;
    m->in2 = RCC_IN2;
    m->in3 = RCC_IN3;
    m->in4 = RCC_IN4;
}


void MotorPower(Motor *m, int lp, int rp)
{
    //--Handle Directionality--//
    if(lp < 0)
    {
        gpio_put(m->in1, 0);
        gpio_put(m->in2, 1);
    }
    else 
    { 
        gpio_put(m->in1, 1);
        gpio_put(m->in2, 0);
    }

    if(rp < 0)
    {
        gpio_put(m->in3, 0);
        gpio_put(m->in4, 1);
    }
    else 
    { 
        gpio_put(m->in3, 1);
        gpio_put(m->in4, 0);
    }

    // pwm_set_dutyF(m->left.slice, m->left.chan, std::abs(lp));
    // pwm_set_dutyF(m->right.slice, m->right.chan, std::abs(rp));
    pwm_set_duty(m->left.gpio, abs(lp));
    pwm_set_duty(m->right.gpio, abs(rp));
}