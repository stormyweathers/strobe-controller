#ifndef PARAMS_H
#define PARAMS_H

#include "strobe_channel.h"
#include <control-panel.h>

uint8_t read_resolution = 8;
uint8_t joystick_x0 =128;
uint8_t joystick_y0 =128;
uint8_t joystick_z0 =128;


controlPanel panel;
bool strobe_enabled = 0;
errorCode err;
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

int fan_pins[] = { LED_FAN_R, LED_FAN_G, LED_FAN_B };
int dance_pins[] = { LED_DANCE_1, LED_DANCE_2, LED_DANCE_3, LED_DANCE_4};
int drip_pins[] = { LED_DRIP_R, LED_DRIP_G, LED_DRIP_B};

// Construct the 3 strobe channels
//    strobe_channel(uint8_t num_subchannels, int pin_numbers[], TeensyTimerTool::TimerGenerator* pulse_timer_id );
strobe_channel   fan(3,   fan_pins, TMR1);
strobe_channel dance(4, dance_pins, TMR2);
strobe_channel  drip(3, drip_pins, TMR3);

//Fields that are communicated from the raspi
int16_t speed =  0;
uint8_t color_mode = 0;
uint8_t freq_mode = 0;
bool    strobe_coin_enabled =0;
bool    strobe_coin_enabled_prev =0;

float transform_matrix[3][3] = {{1,0,0},{0,1,0},{0,0,1}};

#endif