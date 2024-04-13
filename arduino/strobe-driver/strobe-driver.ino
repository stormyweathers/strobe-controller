#include <digitalWriteFast.h>
#include <TeensyTimerTool.h>
using namespace TeensyTimerTool;
#include <math.h>
#include <Bounce2.h>
#include <elapsedMillis.h>

//my custom libs
#include <ControlPanel.h>
#include "globals.h"
#include "pulse_sequences.h"
#include "comms.h"
#include "menu.h"
#include "strobe_channel.h"
#include "timers.h"
#include "transformations.h"
#include "PulseTrain.h"
#include "flicker_match.h"

bool manual_color = false;

void all_on()
{
  Serial.println("Turning strobe timer on!");
  for (int i = 0; i < num_channels; i++){
    channel_list[i]->strobe_timer.start();
  }
  //send_I2C_frequency(dance.fundamental_freq_hz);
  strobe_enabled = 1;
}

void all_off()
{
  Serial.println("Turning strobe timer off!");
  for (int i = 0; i < num_channels; i++){
    channel_list[i]->strobe_timer.stop();
  }
  //send_I2C_frequency(-1);
  strobe_enabled = 0;
}

// Process state and inputs to determine whether to turn on/off
void on_off()
{
  if (!strobe_enabled)
  {
    //When strobes are off, turn them on if:
    // Toggle is up (=0)
    if (!panel.toggle.read()){
      all_on();
    }
    /*
    // toggle is down, AND coin slot has the right edge
    else if ( strobe_coin_enabled  )
    {
      all_on();
    }
    */
  }

  if (strobe_enabled)
  {    
    //When strobes are off, turn them on if:
    // Toggle is DOWN (=1), and coin slot has the right edge
    //if (panel.toggle.read() & !strobe_coin_enabled)
    if (panel.toggle.read())
    {
      all_off();
    } 
  }

}

void setup() {
  //TeensyTimerTool error handler
  attachErrFunc(ErrorHandler(Serial));

  Wire1.begin();

  Serial.println("started panel init");
  panel.resolution = 8;
  panel.init();
  panel.joystick_x0 = 123;
  panel.joystick_y0 = 126;
  panel.joystick_z0 = 123;
  
  fan.pulse_sequence_ptr = &two_tone[0];
  fan.pulse_sequence_size = 2;
  fan.fundamental_freq_hz = 26.9;
  fan.speed_control_range_hz = 3;
  fan.slider_control_range_hz = 3;
  fan.compute_strobe_period(128,0);
  fan.pulse_width_multiple = 1.5;



  Serial.printf("fan period: %i\n",fan.strobe_period_us);

  spot.pulse_sequence_ptr = &two_tone[0];
  spot.pulse_sequence_size = 2;
  spot.fundamental_freq_hz = 100.0;
  spot.speed_control_range_hz = 0.1;
  spot.compute_strobe_period(128,0);
  spot.relative_width_factors[1] = 1.1;
  spot.relative_width_factors[2] = 1.8;
  Serial.printf("spot period: %i\n",spot.strobe_period_us);

  err = fan.initialize_timers();
  err = spot.initialize_timers();

  init_timers();

  //defined in menu.h
  panel.enc.attachButtonCallback(onButtonChanged);
  panel.enc.attachCallback(onRotorChanged);

  Serial.println("initialized");


  //fanColorModulationEnabled = 1;
  //  arc_angle=TWO_PI;
 // fan.numerator = 6;
  color_mode = 2;
  freq_mode = 1;
  //apply_mode_drip(&drip, color_mode, freq_mode);
  apply_mode_fan(&fan, color_mode, freq_mode);
  apply_mode_spot(&spot, color_mode, freq_mode);
  //dance.compute_strobe_period(127,0);
  //send_I2C_frequency(dance.fundamental_freq_hz);

}

uint32_t modulation_period_ms = 5000;
elapsedMillis since_cycle;

void loop() {

  // Read human inputs
  panel.readState();

  //Turn strobes on/off when appropriate
  on_off();

  if (fanColorModulationEnabled && !manual_color)
  {
    if (since_cycle > modulation_period_ms)
    {
     //since_cycle -= modulation_period_ms;
    }
    //Compute angle for modulation
    float angle = map_bounce(float(since_cycle)/modulation_period_ms,0,1,-arc_angle/2, arc_angle/2); 
    colorspace_operations::rotation_matrix_full(angle,colorspace_operations::mat_rot);

    fan.transform_enabled = 1;    
    fan.set_transform_matrix(colorspace_operations::mat_rot);

    spot.transform_enabled = 1;
    spot.set_transform_matrix(colorspace_operations::mat_rot);
  
    //print_mat(mat_rot);
    
  }
  
  // Double click main button to enter flicker match calibration mode
  if ( panel.button.pressed() && ( panel.button.previousDuration() < 250) )
  {
    flicker_handler.run_match(&fan, &panel);
  }
  
  if (panel.joystick_button.fell()){
    //Joystick Cycles through modes, but skip color_mode ==1

    freq_mode++;

    if ( 9 == freq_mode  )
    {
      color_mode = color_mode%3+1;
      freq_mode = 1;
    }

    apply_mode_fan(&fan, color_mode, freq_mode);
    apply_mode_spot(&spot, color_mode, freq_mode);
  }


  if (panel.button.fell() ){
    Serial.println("Main button pressed!");
    manual_color = !manual_color;
    apply_mode_fan(&fan, color_mode, freq_mode);
    apply_mode_spot(&spot, color_mode, freq_mode);
    if (!manual_color)
    {
      //reset the transform to identity matrix when manual color is disabled
      for (uint8_t ii=0; ii<3;ii++){
        for (uint8_t jj=0; jj<3 ;jj++){
          transform_matrix[ii][jj]=0;
          if(ii==jj){
            transform_matrix[ii][jj] = 1;
          }
        }
      }
      fan.hue_param = 0.0;
      //copy_mat(id,transform_matrix);
    }
  }
  
  if (strobe_enabled){

    if (manual_color)
    {
      colorspace_operations::matrix_from_xy(panel.joystick_x,panel.joystick_y,transform_matrix);
      fan.hue_param = panel.joystick_z;
      spot.hue_param = panel.joystick_z;
    }

    fan.compute_strobe_period(panel.analog_in_state[5],speed);
    //No speed control on spotlight
    spot.compute_strobe_period(127,0);
    
    
    /*
    dance_changed = (dance.numerator != dance_numerator_prev) | (dance.denominator != dance_denominator_prev) | (dance.fundamental_freq_hz != dance_fundamental_prev);
    if (dance_changed)
    {
      send_I2C_frequency(dance.fundamental_freq_hz);
      
      dance_numerator_prev =dance.numerator;
      dance_denominator_prev = dance.denominator;
      dance_fundamental_prev = dance.fundamental_freq_hz;
    }
    */

    //Interrupts off
    cli();
    
    fan.update_strobe_period();
    spot.update_strobe_period();
    //dance.update_strobe_period();

    if (manual_color)
    {
      fan.set_transform_matrix(transform_matrix);
      spot.set_transform_matrix(transform_matrix);
    }

    
    modulation_period_ms = 5000.0;// map(float(panel.analog_in_state[4]),0.0, 255.0, 1000.0, 30000.0  );
    mode_timer.setPeriod( mode_period_us );
    spot.pulse_width_multiple = pulse_width_normalization*map(float(panel.analog_in_state[3]),0.0,255.0,0.03125,32.0);
    //Serial.println(map(float(panel.analog_in_state[3]),0.0,255.0,0.03125,32.0));
    //dance.pulse_width_multiple =  map(float(panel.analog_in_state[3]),0.0,255.0,0.03125,16.0);

    //Interrupts on
    sei();
    //Serial.printf("X=%f,Y0=%f,Z0=%f \n",panel.joystick_x, panel.joystick_y, panel.joystick_z);
  }

  //Serial.println(map(float(panel.analog_in_state[4]),0, 255, 1'000'000  , 10'000'000  ));
  mode_period_us = map(float(panel.analog_in_state[4]),0, 255, 500'000  , 999'000'000  );    
  
  if (update_display_flag){
    update_display();
  }
  
  /*
  // Handle communications from UART
  if ( read_line(&read_buff[0])){
    parse_line(&read_buff[0]);
    apply_mode_fan(&fan, color_mode, freq_mode);
    apply_mode_drip(&drip, color_mode, freq_mode);
    apply_mode_dance(&dance, color_mode, freq_mode);
  }
  */

}