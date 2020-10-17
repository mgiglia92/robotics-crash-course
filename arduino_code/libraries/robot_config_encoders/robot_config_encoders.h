#include <Arduino.h>
#define RIGHT_ENCODER 3
#define LEFT_ENCODER 2

class Config{

public:
    Config();
    volatile int right_count;
    volatile int left_count;

    void count_l();
    void count_r();

    void setup_interrupts();
};

extern void cr();