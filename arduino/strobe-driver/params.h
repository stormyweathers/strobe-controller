#ifndef PARAMS_H
#define PARAMS_H

// multuplies the literal integer pulse code to determine
// the pulse width in us
volatile float pulse_width_multiple = 1 ;


//Pin numbers on control panel board

// 3- channels to control the fan
#define LED_FAN_R  4
#define LED_FAN_G  5
#define LED_FAN_B  3

// 4-channels to control the slow-dance
#define LED_DANCE_1  8
#define LED_DANCE_2  10
#define LED_DANCE_3  9
#define LED_DANCE_4  7

//3-channels to control the water drip
//repurposing the 
#define LED_DRIP_R 6
#define LED_DRIP_G 22
#define LED_DRIP_B 23

uint8_t read_resolution = 8;


#endif