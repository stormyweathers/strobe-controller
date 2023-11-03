#include <digitalWriteFast.h>
#include <TeensyTimerTool.h>
using namespace TeensyTimerTool;
#include <math.h>
#include <Bounce2.h>

//my custom libs
#include <control-panel.h>
controlPanel panel;
#include "pulse_sequences.h"
//#include "rhythm.h"
#include "comms.h"
#include "menu.h"
#include "params.h"
#include "strobe_channel.h"


/*
void init_pulse_seq_params()
{
strobe_period_us_fan = 5000;
pulse_count_fan = 0;
pulse_sequence_ptr_fan = &fractal_path_1[0];
pulse_sequence_size_fan = 9;

strobe_period_us_dance = 5000;
pulse_count_dance = 0;
pulse_sequence_ptr_dance = &no_color[0];
pulse_sequence_size_dance = 2;

strobe_period_us_drip = 5000;
pulse_count_drip = 0;
pulse_sequence_ptr_drip = &fractal_path_0[0];
pulse_sequence_size_drip = 3;
}
*/

bool strobe_enabled = 0;

#include "timers.h"

errorCode err;

uint8_t channel_select = 0;

// Construct the 3 strobe channels
//    strobe_channel(uint8_t num_subchannels, int pin_numbers[], TeensyTimerTool::TimerGenerator* pulse_timer_id );
int fan_pins[] = { LED_FAN_R, LED_FAN_G, LED_FAN_B };
int dance_pins[] = { LED_DANCE_1, LED_DANCE_2, LED_DANCE_3, LED_DANCE_4};
int drip_pins[] = { LED_DRIP_R, LED_DRIP_G, LED_DRIP_B};

strobe_channel   fan(3,   fan_pins, TMR1);
strobe_channel dance(4, dance_pins, TMR2);
strobe_channel  drip(3, drip_pins, TMR3);

/*
uint32_t compute_strobe_period(){
  uint32_t coarse = panel.analog_in_state[5];
  float offset_hz = map(coarse,0,255,-5,5);
  if (manual_control){
    float freq_hz = fundamental_hz*float(numerator)/float(denominator);
    return static_cast<uint32_t> (1000000/freq_hz);
  }
  else{
    return static_cast<uint32_t> ( 1000000/map(speed,-1000,1000,140+offset_hz,100-offset_hz) );
  }
}
*/

float transform_matrix[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
float x=0,y=0;
void compute_transform_matrix(){

  // Build a stochastic matrix from the X,Y joystick coordinates

  //Extract s,t parameters from joystick X,Y coords
  x = panel.analog_in_state[0]/(pow(2,read_resolution)-1);
  y = panel.analog_in_state[1]/(pow(2,read_resolution)-1);
  float delta = x+y-1;
  //Fold the unit square across its diagonal
  if (delta>0){
    x=x-delta;
    y=y-delta;
  }

  transform_matrix[0][0]=1-x-y;
  transform_matrix[1][1]=1-x-y;
  transform_matrix[2][2]=1-x-y;

  transform_matrix[1][0]=x;
  transform_matrix[2][1]=x;
  transform_matrix[0][2]=x;

  transform_matrix[2][0]=y;
  transform_matrix[0][1]=y;
  transform_matrix[1][2]=y;
}

uint32_t apply_transform(uint32_t pulse_code){
  uint8_t rgb_val[3] = {WIDTH_R(pulse_code),WIDTH_G(pulse_code),WIDTH_B(pulse_code)};
  uint8_t new_rgb_val[3]={0,0,0};

  // 3x3 matrix multuplication
  for (uint8_t ii=0; ii<3;ii++){
    for (uint8_t jj=0; jj<3 ;jj++){
      new_rgb_val[ii] += transform_matrix[ii][jj]*rgb_val[jj];
    }
  }
  //use the bitmask to return a pulse code
  return ( (new_rgb_val[0] & 0xFF) << 24) | (  (new_rgb_val[1] & 0xFF) << 16) | (  (new_rgb_val[2] & 0xFF << 8) ) ;
}

void setup() {
  //TeensyTimerTool error handler
  attachErrFunc(ErrorHandler(Serial));

  Serial.println("started panel init");
  panel.init();

// These are done manually, because they are repurposing pins that were allocated for analog input on the teensy control panel.
  pinModeFast(LED_DRIP_G, OUTPUT);
  digitalWriteFast(LED_DRIP_G, 0);
  pinModeFast(LED_DRIP_B, OUTPUT);
  digitalWriteFast(LED_DRIP_B, 0);

  analogReadResolution(read_resolution);

  //init_pulse_seq_params();
  init_timers();

  err = fan.initialize_timers();
  err = drip.initialize_timers();
  err = dance.initialize_timers();

  fan.pulse_sequence_ptr = &fractal_path_1[0];
  fan.pulse_sequence_size = 9;
  fan.fundamental_freq_hz = 7.5;

  dance.pulse_sequence_ptr = &no_color[0];
  dance.pulse_sequence_size = 4;
  dance.fundamental_freq_hz = 30.0;

  drip.pulse_sequence_ptr = &fractal_path_0[0];
  drip.pulse_sequence_size = 3;
  drip.fundamental_freq_hz = 60.0;

  //defined in menu.h
  panel.enc.attachButtonCallback(onButtonChanged);
  panel.enc.attachCallback(onRotorChanged);

  Serial.println("initialized");
}

void loop() {

  // Read human inputs
  panel.readState();

  //Process human inputs

  // Master shutoff switch for light triggers
  if (panel.toggle.rose()){
    Serial.println("Turning strobe timer off!");
    fan.strobe_timer.stop();
    dance.strobe_timer.stop();
    drip.strobe_timer.stop();
    strobe_enabled = 0;
  }
  if (panel.toggle.fell()){
    Serial.println("Turning strobe timer on!");
    fan.strobe_timer.start();
    dance.strobe_timer.start();
    drip.strobe_timer.start();
    strobe_enabled = 1;
  }

  if (panel.joystick_button.fell()){
    //button press toggles between RATIONAL/IRRATIONAL multiples
    manual_control = !manual_control;
  }

  // Switch between different pulse sequences
  if (panel.button.fell() ){
    Serial.println("Main button pressed!");
    
    channel_select = (channel_select+ 1) % 3;
    
  }
  if (panel.button.rose() ){
  }

  if (strobe_enabled){
    compute_transform_matrix();

    fan.compute_strobe_period(panel.analog_in_state[1],speed);
    drip.compute_strobe_period(128,speed);
    dance.compute_strobe_period(128,speed);
    //Interrupts off
      
    
    cli();
    fan.update_strobe_period();
    drip.update_strobe_period();
    dance.update_strobe_period();

    fan.pulse_width_multiple =  map(float(panel.analog_in_state[3]),0.0,255.0,0.03125,4.0);
    drip.pulse_width_multiple =  map(float(panel.analog_in_state[3]),0.0,255.0,0.03125,4.0);
    dance.pulse_width_multiple =  map(float(panel.analog_in_state[3]),0.0,255.0,0.03125,4.0);
    
    //Interrupts on
    sei();
    //Serial.println(pulse_width_multiple);
  }

  if (update_display_flag){
    update_display();
  }

  if ( read_line(&read_buff[0])){
    parse_line(&read_buff[0]);
  }
}

void update_display(){
  panel.display.clearDisplay();
  panel.display.setCursor(0,0);
  // 21 chars per line

  panel.display.println("   Fan     Dan     Dr");
  panel.display.println("---------------------");
  panel.display.println("f0 (Hz)");
  panel.display.println(" xxx.xx xxx.xx xxx.xx");

  if (enc_select_mode == FUNDAMENTAL)
  {
    highlight_frequency(decimal_place,channel_select);
  }
  else{  panel.display.println();  }
  
  panel.display.println("---------------------");
  panel.display.println("ratio");
  panel.display.println("  a/b     a/b     a/b");

  if (enc_select_mode == RATIO)
  {
    highlight_ratio(fraction_component,channel_select);
  }
  else{  panel.display.println();  }
  

  panel.display.println("---------------------");
  panel.display.println("f (Hz)");
  panel.display.println(" xxx.xx xxx.xx xxx.xx");
  panel.display.println("---------------------");
  panel.display.println("     last msg     ");
  panel.display.println("Speed   Color    Freq");
  panel.display.printf("%04d     %1d        %1d",speed,color_mode,freq_mode);

  panel.display.display();
}

void highlight_frequency(int8_t decimal_place, uint8_t channel_select)
{
  //line format "|xxx.xx|xxx.xx|xxx.xx"
  print_spaces(1+7*channel_select+2-decimal_place + (decimal_place < 0) );
  //Add an extra space for the decimal point
  
  panel.display.println("^");
}

void highlight_ratio(bool fraction_component, uint8_t channel_select)
{
  //Line format ..a/b.....a/b.....a/b
  print_spaces(2+8*channel_select);
  panel.display.println(fraction_component == NUMERATOR ? "^  " : "  ^");
}

void print_spaces(uint8_t num_spaces)
{
  for (int i =0; i< num_spaces; i++)
  {
    panel.display.print(" ");
  }
}


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