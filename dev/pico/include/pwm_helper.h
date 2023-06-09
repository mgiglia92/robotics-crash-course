#include "hardware/pwm.h"

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