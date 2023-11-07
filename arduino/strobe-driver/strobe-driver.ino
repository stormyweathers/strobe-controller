#include <digitalWriteFast.h>
#include <TeensyTimerTool.h>
using namespace TeensyTimerTool;
#include <math.h>
#include <Bounce2.h>
#include <elapsedMillis.h>

//my custom libs
#include <control-panel.h>
#include "globals.h"
#include "pulse_sequences.h"
#include "comms.h"
#include "menu.h"
#include "strobe_channel.h"
#include "timers.h"
#include "transformations.h"

bool manual_color = false;

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
  fan.fundamental_freq_hz = 29.0;
  fan.speed_control_range_hz = 3;
  fan.compute_strobe_period(128,0);
  Serial.printf("fan period: %i\n",fan.strobe_period_us);

  dance.pulse_sequence_ptr = &cycle[0];
  dance.pulse_sequence_size = 4;
  dance.fundamental_freq_hz = 30.0;
  dance.speed_control_range_hz = 10;
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


  fanColorModulationEnabled = 1;
//  arc_angle=TWO_PI;
 // fan.numerator = 6;
}

uint32_t modulation_period_ms = 5000;
elapsedMillis since_cycle;


void loop() {
  if (since_cycle > modulation_period_ms)
  {
    since_cycle -= modulation_period_ms;
  }
  if (fanColorModulationEnabled)
  {
    //Compute angle for modulation
    float angle = map_bounce(float(since_cycle)/modulation_period_ms,0,1,-arc_angle/2, arc_angle/2); 
    //Serial.println(angle);
    fan.transform_enabled = 1;
    rotation_matrix_full(angle,mat_rot);
    fan.set_transform_matrix(mat_rot);
    //print_mat(mat_rot);
  }

  // Read human inputs
  panel.readState();

  //Detect edges on the strobe_coin_enabled message sent from raspi
  bool coin_turn_on = strobe_coin_enabled & !strobe_coin_enabled_prev;
  bool coin_turn_off = !strobe_coin_enabled & strobe_coin_enabled_prev;

  if (coin_turn_on){
    Serial.println("Coin on!");
  }
  if (coin_turn_off){
    Serial.println("Coin off!");
  }


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
    //
    speed = map(panel.analog_in_state[4],0,255,-1000.0,1000.0);
  }

  if (panel.joystick_button.isPressed())
  {
    Serial.printf("duration: %i \n",panel.joystick_button.currentDuration());
  }
  if (panel.joystick_button.isPressed() & (panel.joystick_button.currentDuration() > 250)  )
  {
    //Long joystick button press toggles color transformation enable
    Serial.println("joystick long press!");
    fan.transform_enabled = !fan.transform_enabled;
    drip.transform_enabled = !drip.transform_enabled;
  }

  // cycle which channel is controlled by the encoder
  if (panel.button.fell() ){
    Serial.println("Main button pressed!");
    channel_select = (channel_select+ 1) % 3;
  }

  if (panel.button.rose() ){
  }

  if (strobe_enabled){

    if (manual_color)
    {
      matrix_from_xy(panel.joystick_x,panel.joystick_y);
    }

    fan.compute_strobe_period(panel.analog_in_state[5],speed);
    drip.compute_strobe_period(127,speed);

    //For slow dance, the speed is set by driving the 
    dance.compute_strobe_period(127,speed);
    // factor of 1.06 was measured to get them to match...
    dance_changed = (dance.numerator != dance_numerator_prev) | (dance.denominator != dance_denominator_prev) | (dance.fundamental_freq_hz != dance_fundamental_prev);
    if (dance_changed)
    {

      drive_frequency =  1.06*( dance.fundamental_freq_hz);
      Serial.printf("dance freq: %f\n", dance.freq_hz);
      Serial.printf("sending I2C freq: %f\n", drive_frequency);
      send_I2C_frequency(drive_frequency);

      dance_numerator_prev =dance.numerator;
      dance_denominator_prev = dance.denominator;
      dance_fundamental_prev = dance.fundamental_freq_hz;
    }
    


    //Interrupts off
    cli();
    fan.update_strobe_period();
    drip.update_strobe_period();
    dance.update_strobe_period();

    if (manual_color)
    {
      fan.set_transform_matrix(transform_matrix);
      drip.set_transform_matrix(transform_matrix);
    }

    fan.pulse_width_multiple =  map(float(panel.analog_in_state[3]),0.0,255.0,0.03125,4.0);
    drip.pulse_width_multiple = 4;
    dance.pulse_width_multiple =  map(float(panel.analog_in_state[3]),0.0,255.0,0.03125,4.0);
    
    //Interrupts on
    sei();
    //Serial.printf("X=%f,Y0=%f,Z0=%f \n",panel.joystick_x, panel.joystick_y, panel.joystick_z);
  }

  if (update_display_flag){
    update_display();
  }

  // Hande communications

  if ( read_line(&read_buff[0])){
    parse_line(&read_buff[0]);
    apply_mode_fan(color_mode, freq_mode);
    apply_mode_drip(color_mode, freq_mode);
  }

  
}