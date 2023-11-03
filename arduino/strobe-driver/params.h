#ifndef PARAMS_H
#define PARAMS_H

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

uint8_t read_resolution = 8;


#endif