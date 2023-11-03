/* For defining the menu navigation and subsequent displays
 *
 */
 #ifndef MENU_H
 #define MENU_H

// Main mode selection
#define FUNDAMENTAL 0
#define RATIO 1
#define NUMBER_OF_MODES 2

uint32_t last_enc_press = 0;
uint32_t long_press_thresh_ms = 250;
uint8_t enc_select_mode = RATIO;
bool manual_control = true;
//Fundamental Freq control
float fundamental_hz = 30.0;
int8_t decimal_place = 0; 

//Encoder ratio control
bool NUMERATOR = 1;
bool DENOMINATOR = 0;
bool RATIONAL = 0;
bool IRRATIONAL = 1;
bool fraction_component = NUMERATOR;
bool coefficient = 0;
uint16_t numerator = 1;
uint16_t denominator = 1;

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
      fundamental_hz += delta*pow(10,decimal_place);
      fundamental_hz = constrain(fundamental_hz, 1,1500);
      break;
    case RATIO: 
      if (fraction_component == NUMERATOR){
        numerator += delta;
        numerator = max(numerator,1);
      }
      if (fraction_component == DENOMINATOR){
        denominator += delta; 
        denominator = max(denominator,1);
      }
      break;
    default:
      break;
  }
}

 #endif