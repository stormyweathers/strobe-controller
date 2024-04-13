#ifndef PARAMS_H
#define PARAMS_H

#include "strobe_channel.h"
#include <ControlPanel.h>
#include "flicker_match.h"
controlPanel panel;
bool strobe_enabled = 0;
TeensyTimerTool::errorCode err;
//Pin numbers on control panel board


// 3- channels to control the fan
#define LED_FAN_R  4
#define LED_FAN_G  5
#define LED_FAN_B  3
int fan_pins[] = { LED_FAN_R, LED_FAN_G, LED_FAN_B };
strobe_channel   fan(3,   fan_pins, TeensyTimerTool::TMR1,TeensyTimerTool::TMR1);
bool fanColorModulationEnabled = false;

//3-channels to control the spotlight
#define LED_SPOT_R 6
#define LED_SPOT_G 22
#define LED_SPOT_B 23
int spot_pins[] = { LED_SPOT_R, LED_SPOT_G, LED_SPOT_B};
strobe_channel  spot(3, spot_pins, TeensyTimerTool::TMR3,TeensyTimerTool::TMR3);

/*
int dance_pins[] = { LED_DANCE_1, LED_DANCE_2, LED_DANCE_3, LED_DANCE_4};
strobe_channel dance(4, dance_pins, TeensyTimerTool::TMR4,TeensyTimerTool::TMR2);
float drive_frequency = 30.0;
bool dance_changed = false;
uint16_t dance_numerator_prev = 1 ;
uint16_t dance_denominator_prev = 1;
uint16_t dance_fundamental_prev = 1;
*/

uint8_t num_channels = 2;
strobe_channel* channel_list[2] = {&fan, &spot};


float frequency_minimum_hz = 18.0; //Determined by type of timer used 


//Fields that are communicated from the raspi
int16_t speed =  0;
uint8_t color_mode = 0;
uint8_t freq_mode = 0;
bool    strobe_coin_enabled =0;
bool    strobe_coin_enabled_prev =0;

float transform_matrix[3][3] = {{1,0,0},{0,1,0},{0,0,1}};

float arc_angle = TWO_PI;

bool update_display_flag = false;

flicker_match flicker_handler(&update_display_flag);

uint32_t mode_period_us;


float map_bounce(float val, float old_min, float old_max, float new_min, float new_max)
//Modification of the map function to reflect off boundaries
{
  float diff;
    if (val > old_max)
    {
        diff = -2*(val-old_max);    
        return map_bounce( val+diff, old_min,old_max,new_min,new_max     );
    }
    if (val < old_min)
    {  
        diff = 2*(old_min-val);
        return map_bounce(val+diff,old_min,old_max,new_min,new_max);
    }
    return (val - old_min) * (new_max - new_min) / (old_max - old_min) + old_min;
}

float map_wrap(float val, float old_min, float old_max, float new_min, float new_max)
//Modification of the map function to reflect off boundaries
{
  float diff;
    if (val > old_max)
    {
        diff = -old_max;
        return map_wrap( val+diff, old_min,old_max,new_min,new_max     );
    }
    if (val < old_min)
    {  
        diff = old_min;
        return map_wrap(val+diff,old_min,old_max,new_min,new_max);
    }
    return (val - old_min) * (new_max - new_min) / (old_max - old_min) + old_min;
}


bool coin_turn_on = 0;
bool coin_turn_off =0;
uint8_t last_coin_edge = 0;

#endif