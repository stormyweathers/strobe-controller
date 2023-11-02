#ifndef PARAMS_H
#define PARAMS_H

// multuplies the literal integer pulse code to determine
// the pulse width in us
volatile float pulse_width_multiple = 1 ;


//Pin numbers on control panel board
#define LED_R  4
#define LED_G  5
#define LED_B  3
#define LED_W  6

#define LED_1  8
#define LED_2  9
#define LED_3  7
#define LED_4  10

uint8_t read_resolution = 8;

#endif