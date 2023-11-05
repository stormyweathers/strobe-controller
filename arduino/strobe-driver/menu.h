/* For defining the menu navigation and subsequent displays
 *
 */
 #ifndef MENU_H
 #define MENU_H

#include "strobe_channel.h"
#include "globals.h"

// Main mode selection
#define FUNDAMENTAL 0
#define RATIO 1
#define NUMBER_OF_MODES 2

uint32_t last_enc_press = 0;
uint32_t long_press_thresh_ms = 250;
uint8_t enc_select_mode = RATIO;
bool manual_control = true;
int8_t decimal_place = 0; 

//Encoder ratio control
bool NUMERATOR = 1;
bool DENOMINATOR = 0;
bool RATIONAL = 0;
bool IRRATIONAL = 1;
bool fraction_component = NUMERATOR;


// Incremented by pressing main button
uint8_t channel_select = 0;

strobe_channel* channel_list[3];


//bool coefficient = 0;
//uint16_t numerator = 1;
//uint16_t denominator = 1;

void onButtonChanged(int state){
  if (state == 0){
    // Start press
    switch(enc_select_mode)
    {
      case FUNDAMENTAL:
        decimal_place--;
        if (decimal_place < -2){
          decimal_place = +2;
        }
        break;
      case RATIO:
        fraction_component = ! fraction_component;
        break;
      default:
        break;
    }
    
    last_enc_press = millis();
  }
  else{
    //Release
    if ( (millis() - last_enc_press)  > long_press_thresh_ms ){
      enc_select_mode= (enc_select_mode+1)%NUMBER_OF_MODES;
      //Serial.println(enc_select_mode);
    }
  }
}
void onRotorChanged(int state, int delta){
  switch(enc_select_mode)
  {
    case FUNDAMENTAL:

      channel_list[channel_select]->fundamental_freq_hz += delta*pow(10,decimal_place);
      channel_list[channel_select]->fundamental_freq_hz = constrain(channel_list[channel_select]->fundamental_freq_hz, 1,1500);
      break;
    case RATIO: 
      if (fraction_component == NUMERATOR){
        channel_list[channel_select]->numerator += delta;
        channel_list[channel_select]->numerator = max(channel_list[channel_select]->numerator,1);
      }
      if (fraction_component == DENOMINATOR){
        channel_list[channel_select]->denominator += delta; 
        channel_list[channel_select]->denominator = max(channel_list[channel_select]->denominator,1);
      }
      break;
    default:
      break;
  }
}

void print_spaces(uint8_t num_spaces)
{
  for (int i =0; i< num_spaces; i++)
  {
    panel.display.print(" ");
  }
}

void highlight_ratio(bool fraction_component, uint8_t channel_select)
{
  //Line format ..a/b.....a/b.....a/b
  print_spaces(2+7*channel_select);
  panel.display.println(fraction_component == NUMERATOR ? "^  " : "   ^");
}

void update_display(){
  panel.display.clearDisplay();
  panel.display.setCursor(0,0);
  // 21 chars per line
  panel.display.println("   Fan     Dan     Dr");
  panel.display.println("---------------------");
  panel.display.println("f0 (Hz)");
  panel.display.printf(" %6.2f %6.2f %6.2f\n",fan.fundamental_freq_hz, dance.fundamental_freq_hz, drip.fundamental_freq_hz);
  //panel.display.println("|xxx.xx|xxx.xx|xxx.xx");

  if (enc_select_mode == FUNDAMENTAL)
  {
    //line format "|xxx.xx|xxx.xx|xxx.xx"
    //Add an extra space for the decimal point
    print_spaces(1+7*channel_select+2-decimal_place + (decimal_place < 0) );
    panel.display.println("^");
  }
  else{  panel.display.println();  }
  
  panel.display.println("---------------------");
  panel.display.println("ratio");
  //panel.display.println("..a/b.....a/b.....a/b");
  //panel.display.println("aaa/bbb.aaa/bbb.aaa/b");
  panel.display.printf("%3i/%3i %2i/%2i  %2i/%2i\n",fan.numerator,fan.denominator,dance.numerator,dance.denominator,drip.numerator,drip.denominator);

  if (enc_select_mode == RATIO)
  {
    highlight_ratio(fraction_component,channel_select);
  }
  else{  panel.display.println();  }
  
  panel.display.println("---------------------");
  panel.display.println("f (Hz)");
  panel.display.printf(" %6.1f %6.1f %6.1f\n",fan.freq_hz, dance.freq_hz, drip.freq_hz);
  panel.display.println("---------------------");
  panel.display.println("     last msg     ");
  panel.display.println("Speed   Color    Freq");
  panel.display.printf("%04d     %1d        %1d",speed,color_mode,freq_mode);

  panel.display.display();
}
 #endif