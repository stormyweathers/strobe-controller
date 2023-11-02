#include <digitalWriteFast.h>
#include <TeensyTimerTool.h>
using namespace TeensyTimerTool;
#include <math.h>
#include <Bounce2.h>

//my custom libs
#include <control-panel.h>
controlPanel panel;
#include "pulse_sequences.h"
#include "rhythm.h"
#include "comms.h"
#include "menu.h"
#include "params.h"
#include "timers.h"

volatile uint32_t strobe_period_us = 5000;
volatile uint32_t pulse_count = 0;
// Choose a default pulse sequence
volatile uint32_t *pulse_sequence_ptr = &cycle_path[0];
uint16_t pulse_sequence_size = 4;


volatile uint32_t strobe_period_us_2 = 5000;
volatile uint32_t pulse_count_2 = 0;
volatile uint32_t *pulse_sequence_ptr_2 = &pulse_5[0];
uint16_t pulse_sequence_size_2 = 5;
bool strobe_enabled = 0;

errorCode err;

uint8_t button_press_count = 0;

void single_pulse(){
  pulse_code = *(pulse_sequence_ptr+pulse_count%pulse_sequence_size);
  //pulse_code = apply_transform(pulse_code);
  
  if (rhythm_buff[pulse_count%rhythm_size]) {
    digitalWriteFast(LED_R,1);
    timer_r.trigger(int( pulse_width_multiple*WIDTH_R(pulse_code)) );
    digitalWriteFast(LED_G,HIGH);
    timer_g.trigger(int( pulse_width_multiple*WIDTH_G(pulse_code)) );
    digitalWriteFast(LED_B,HIGH);
    timer_b.trigger(int( pulse_width_multiple*WIDTH_B(pulse_code)) );
    digitalWriteFast(LED_W,HIGH);
    timer_w.trigger(int( pulse_width_multiple*WIDTH_W(pulse_code)) );
  }
  pulse_count++;
}

void single_pulse_2(){
  pulse_code_2 = *(pulse_sequence_ptr_2+pulse_count_2%pulse_sequence_size_2);
  //pulse_code = apply_transform(pulse_code);
  
  if (rhythm_buff[pulse_count_2%rhythm_size]) {
    digitalWriteFast(LED_1,1);
    timer_1.trigger(int( pulse_width_multiple*WIDTH_R(pulse_code_2)) );
    digitalWriteFast(LED_2,HIGH);
    timer_2.trigger(int( pulse_width_multiple*WIDTH_G(pulse_code_2)) );
    digitalWriteFast(LED_3,HIGH);
    timer_3.trigger(int( pulse_width_multiple*WIDTH_B(pulse_code_2)) );
    digitalWriteFast(LED_4,HIGH);
    timer_4.trigger(int( pulse_width_multiple*WIDTH_W(pulse_code_2)) );
  }
  pulse_count_2++;
}

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
  analogReadResolution(read_resolution);

  // begin timer in stopped mode
  strobe_timer.begin(single_pulse,strobe_period_us, false);
  strobe_timer_2.begin(single_pulse_2,strobe_period_us_2, false);

  timer_r.begin([] { digitalWriteFast(LED_R, LOW); });
  timer_g.begin([] { digitalWriteFast(LED_G, LOW); });
  timer_b.begin([] { digitalWriteFast(LED_B, LOW); });
  timer_w.begin([] { digitalWriteFast(LED_W, LOW); });

  timer_1.begin([] { digitalWriteFast(LED_1, LOW); });
  timer_2.begin([] { digitalWriteFast(LED_2, LOW); });
  timer_3.begin([] { digitalWriteFast(LED_3, LOW); });
  timer_4.begin([] { digitalWriteFast(LED_4, LOW); });

  //Initialize rhythm bufer
  euclidean_rhythm(8,8);
  Serial.println("initialized");

  //Read the panel inputs every 1ms
  //This frequency is necessary for the encoder to register pulses
  read_timer.begin(read_timer_callback,1000);

  display_timer.begin(display_timer_callback,100000);

  //defined in menu.h
  panel.enc.attachButtonCallback(onButtonChanged);
  panel.enc.attachCallback(onRotorChanged);
}

void loop() {

  // Read human inputs
  panel.readState();

  //Process human inputs

  // Master shutoff switch for light triggers
  if (panel.toggle.rose()){
    Serial.println("Turning strobe timer off!");
    strobe_timer.stop();
    strobe_timer_2.stop();
    strobe_enabled = 0;
  }
  if (panel.toggle.fell()){
    Serial.println("Turning strobe timer on!");
    strobe_timer.start();
    strobe_timer_2.start();
    strobe_enabled = 1;
  }

  if (panel.joystick_button.fell()){
    //button press toggles between RATIONAL/IRRATIONAL multiples
    manual_control = !manual_control;
  }

  // Switch between different pulse sequences
  if (panel.button.fell() ){
    Serial.println("Main button pressed!");
    button_press_count = (button_press_count+ 1) % 7;
    pulse_sequence_ptr = fractal_path_ptrs[button_press_count];
    pulse_sequence_size = fractal_path_lengths[button_press_count];
    
  }
  if (panel.button.rose() ){
  }

  if (strobe_enabled){
    compute_transform_matrix();
    //Interrupts off
    cli();
    strobe_period_us = compute_strobe_period();
    err = strobe_timer.setPeriod(strobe_period_us);

    strobe_period_us_2 = compute_strobe_period();    
    err = strobe_timer_2.setPeriod(strobe_period_us_2);

    pulse_width_multiple =  map(float(panel.analog_in_state[3]),0.0,255.0,0.03125,4.0);
    
    //Interrupts on
    sei();
    Serial.println(pulse_width_multiple);
  }

  if (update_display_flag){
    update_display();
  }

  if ( read_line(&read_buff[0])){
    parse_line(&read_buff[0]);
  }
}

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

      panel.display.printf("Freq: %07.2f Hz\n",1000000.0/strobe_period_us);
      panel.display.printf("      %07.1f BPM\n",60*1000000.0/strobe_period_us);
      panel.display.printf("      %07i us\n",strobe_period_us);

      panel.display.print("Control Mode: ");
      panel.display.println(manual_control == true ? "Manual" : "Serial");
      panel.display.println();

      panel.display.print("fractal_path_");
      panel.display.print(button_press_count);
      panel.display.print(" (");
      panel.display.print(pulse_sequence_size);
      panel.display.println(")");

      panel.display.print("pulse_count: ");
      panel.display.println(pulse_count);
      panel.display.print("Speed: ");
      panel.display.println(speed);
      panel.display.print("Mode: ");
      panel.display.println(mode);
      panel.display.display();
      panel.display.display();
}