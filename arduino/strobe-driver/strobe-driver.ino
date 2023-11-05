#include <digitalWriteFast.h>
#include <TeensyTimerTool.h>
using namespace TeensyTimerTool;
#include <math.h>
#include <Bounce2.h>

//my custom libs
#include <control-panel.h>


#include "globals.h"
#include "pulse_sequences.h"
#include "comms.h"
#include "menu.h"

#include "strobe_channel.h"
#include "timers.h"

bool strobe_enabled = 0;
errorCode err;


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
  transform_matrix[1][0]=x;
  transform_matrix[2][0]=y;

  transform_matrix[0][1]=y;
  transform_matrix[1][1]=1-x-y;
  transform_matrix[2][1]=x;

  transform_matrix[0][2]=x;
  transform_matrix[1][2]=y;
  transform_matrix[2][2]=1-x-y;
}

uint32_t apply_transform(uint32_t pulse_code, float transform_matrix[3][3]){
  uint8_t rgb_val[3] ={0,0,0}; //{WIDTH_R(pulse_code),WIDTH_G(pulse_code),WIDTH_B(pulse_code)};
  uint8_t new_rgb_val[3]={0,0,0};
  uint8_t position;

  //Extract pulse code
  for (int i=0; i<3; i++)
  {
    position = 3-i;
    uint32_t mask = 0xFF << (8*position);
    rgb_val[i] =  ( static_cast<uint8_t>(  ( pulse_code & mask ) >> (8*position)  ) );
  }

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

// These are done manually, because they are repurposing pins that were allocated for analog input on the teensy control panel
/*
  pinModeFast(LED_DRIP_G, OUTPUT);
  digitalWriteFast(LED_DRIP_G, 0);
  pinModeFast(LED_DRIP_B, OUTPUT);
  digitalWriteFast(LED_DRIP_B, 0);
*/
  analogReadResolution(read_resolution);

  fan.pulse_sequence_ptr = &fractal_path_0[0];
  fan.pulse_sequence_size = 3;
  fan.fundamental_freq_hz = 29.0;
  fan.speed_control_range_hz = 3;
  fan.compute_strobe_period(128,0);
  Serial.printf("fan period: %i\n",fan.strobe_period_us);

  dance.pulse_sequence_ptr = &all_strobe[0];
  dance.pulse_sequence_size = 2;
  dance.fundamental_freq_hz = 30.0;
  dance.speed_control_range_hz = 2;
  dance.compute_strobe_period(128,0);
  Serial.printf("dance period: %i\n",dance.strobe_period_us);

  drip.pulse_sequence_ptr = &fractal_path_6[0];
  drip.pulse_sequence_size = 2187;
  drip.fundamental_freq_hz = 60.0;
  drip.speed_control_range_hz = 10;
  drip.compute_strobe_period(128,0);
  //drip.rhythm_slope = 1/phi;
  Serial.printf("drip period: %i\n",drip.strobe_period_us);

  err = fan.initialize_timers();
  err = drip.initialize_timers();
  err = dance.initialize_timers();
  init_timers();

  //defined in menu.h
  panel.enc.attachButtonCallback(onButtonChanged);
  panel.enc.attachCallback(onRotorChanged);

  channel_list[0]=&fan;
  channel_list[1]=&dance;
  channel_list[2]=&drip;

  Serial.println("initialized");
  Serial.println( apply_transform(0x00FF0000,transform_matrix),HEX );
}

void loop() {

  // Read human inputs
  panel.readState();


  //Detect edges on the strobe_coin_enabled message sent from raspi
  bool coin_turn_on = strobe_coin_enabled & !strobe_coin_enabled_prev;
  bool coin_turn_off = !strobe_coin_enabled & strobe_coin_enabled_prev;

  // Decide when to toggle main strobe timers
  // Turn on when:
  //  strobe timer is off AND ( coin slot state has just turned on OR master switch is on)
  if (  (!strobe_enabled) & ( (!panel.toggle.read()) | coin_turn_on ) ){
    Serial.println("Turning strobe timer on!");
    fan.strobe_timer.start();
    dance.strobe_timer.start();
    drip.strobe_timer.start();
    strobe_enabled = 1;
  }

  // Turn off when:
  //  strobe timer is on AND ( coin slot state has just turned off OR master switch is off)
  if ( strobe_enabled & ( coin_turn_off | panel.toggle.read() ) ){
    Serial.println("Turning strobe timer off!");
    fan.strobe_timer.stop();
    dance.strobe_timer.stop();
    drip.strobe_timer.stop();
    strobe_enabled = 0;
  }

  if (panel.joystick_button.fell()){
    //button press toggles between RATIONAL/IRRATIONAL multiples
    //manual_control = !manual_control;
    speed = map(panel.analog_in_state[4],0,255,-1000.0,1000.0);
  }

  // cycle which channel is controlled by the encoder
  if (panel.button.fell() ){
    Serial.println("Main button pressed!");
    channel_select = (channel_select+ 1) % 3;
  }

  if (panel.button.rose() ){
  }

  if (strobe_enabled){
    compute_transform_matrix();

    fan.compute_strobe_period(panel.analog_in_state[5],speed);
    drip.compute_strobe_period(127,speed);
    dance.compute_strobe_period(127,speed);

    //Interrupts off
    cli();

    fan.update_strobe_period();
    drip.update_strobe_period();
    dance.update_strobe_period();

    fan.pulse_width_multiple =  map(float(panel.analog_in_state[3]),0.0,255.0,0.03125,4.0);
    drip.pulse_width_multiple = 4;
    dance.pulse_width_multiple =  map(float(panel.analog_in_state[3]),0.0,255.0,0.03125,4.0);
    
    //Interrupts on
    sei();
  }

  if (update_display_flag){
    update_display();
  }

  if ( read_line(&read_buff[0])){
    parse_line(&read_buff[0]);
    apply_mode_fan(color_mode, freq_mode);
    apply_mode_drip(color_mode, freq_mode);
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