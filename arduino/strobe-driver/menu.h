/* For defining the menu navigation and subsequent displays
 *
 */
 #ifndef MENU_H
 #define MENU_H
 
#include "globals.h"
#include "strobe_channel.h"


// Main mode selection
#define FUNDAMENTAL 0
#define RATIO 1
#define NUMBER_OF_MODES 2

uint32_t last_enc_press = millis();
uint32_t last_enc_release = millis();
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




//bool coefficient = 0;
//uint16_t numerator = 1;
//uint16_t denominator = 1;

void onButtonChanged(int state){
  if (state == 0){
    //Double press detection
    if ( (millis() - last_enc_release)  < long_press_thresh_ms ){
      channel_select = (channel_select+ 1) % num_channels;
      last_enc_press = millis();
      return;
    }

    // single press
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
    return;
  }

  else{
    //Release

    //Long press detection
    if ( (millis() - last_enc_press)  > long_press_thresh_ms ){
      enc_select_mode= (enc_select_mode+1)%NUMBER_OF_MODES;
      //Serial.println(enc_select_mode);
    }
    last_enc_release = millis();
  }
}
void onRotorChanged(int state, int delta){
  switch(enc_select_mode)
  {
    case FUNDAMENTAL:

      channel_list[channel_select]->fundamental_freq_hz += delta*pow(10,decimal_place);
      channel_list[channel_select]->fundamental_freq_hz = constrain(channel_list[channel_select]->fundamental_freq_hz, frequency_minimum_hz,1500);
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

void update_display()
{
  // Takes too long, requires yield()

  
  panel.display.clearDisplay();
  
  panel.display.setCursor(0,0);
  
  // 21 chars per line
  panel.display.println("  Fan    Spot     XXX");
  panel.display.println("---------------------");
  panel.display.println("f0 (Hz)");
  panel.display.printf(" %6.2f %6.2f %6.2f\n",channel_list[0]->fundamental_freq_hz, channel_list[1]->fundamental_freq_hz,0.0);
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
  panel.display.printf("%3i/%3i %2i/%2i %2i/%2i\n",channel_list[0]->numerator,channel_list[0]->denominator,channel_list[1]->numerator,channel_list[1]->denominator,0,0);
  

  if (enc_select_mode == RATIO)
  {
    highlight_ratio(fraction_component,channel_select);
  }
  else{  panel.display.println();  }
  
  panel.display.println("---------------------");
  panel.display.println("f (Hz)");
  panel.display.printf(" %6.1f %6.1f %6.1f\n",channel_list[0]->freq_hz, channel_list[1]->freq_hz,0.0);
  panel.display.println("---------------------");
  panel.display.printf("Strobe Enabled:%i\n",strobe_enabled);
  panel.display.println("Speed Color Freq coin");
  panel.display.printf("%04d   %1d     %1d   %1d,%1d",speed,color_mode,freq_mode,strobe_coin_enabled,last_coin_edge);
  
  panel.display.display();
  
  update_display_flag = false;
}

/*
void update_display()
{
  // Takes too long, requires yield()

  
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
  panel.display.printf("strb:%i,tgl:%i,coil:%i%i \n",strobe_enabled,panel.toggle.read(),coin_turn_on,coin_turn_off);
  panel.display.println("Speed Color Freq coin");
  panel.display.printf("%04d   %1d     %1d   %1d,%1d",speed,color_mode,freq_mode,strobe_coin_enabled,last_coin_edge);
  
  panel.display.display();
  
  update_display_flag = false;
}
*/

/*
void update_display(){
  // Write the current status to the oled
      panel.display.clearDisplay();
      panel.display.setCursor(0,0);
      //panel.reportStateOled(false);

      panel.display.printf("Fund (Hz): %07.2f \n",fundamental_hz  );
      if (enc_select_mode == FUNDAMENTAL)  {
        panel.display.print("            ");
        for (int i = decimal_place-3;i<-1;i++   ){
          panel.display.print(" ");
        }
        if (decimal_place < 0){
          panel.display.print(" ");
        }
        panel.display.println("*");

      } 
      else{
        panel.display.println();
      }

      panel.display.print("Freq Ratio: ");
      panel.display.print(numerator);
      panel.display.print(" / ");
      panel.display.println(denominator);
      if ( (fraction_component == NUMERATOR ) & (enc_select_mode == RATIO) ){
        panel.display.println("           ***");
      }
      else if ( (fraction_component == DENOMINATOR) & (enc_select_mode == RATIO)){
        panel.display.println("               ***");
      }
      else{
        panel.display.println();
      }

      panel.display.printf("Freq: %07.2f Hz\n",1000000.0/strobe_period_us_fan);
      panel.display.printf("      %07.1f BPM\n",60*1000000.0/strobe_period_us_fan);
      panel.display.printf("      %07i us\n",strobe_period_us_fan);

      panel.display.print("Control Mode: ");
      panel.display.println(manual_control == true ? "Manual" : "Serial");
      panel.display.println();

      panel.display.print("Speed: ");
      panel.display.println(speed);
      panel.display.print("Color Mode: ");
      panel.display.println(color_mode);
      panel.display.print("Freq Mode: ");
      panel.display.println(freq_mode);
      panel.display.display();
      panel.display.display();
}
*/

 #endif