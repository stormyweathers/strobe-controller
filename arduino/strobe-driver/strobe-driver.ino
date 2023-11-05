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
#include "transformations.h"

void setup() {
  //TeensyTimerTool error handler
  attachErrFunc(ErrorHandler(Serial));

  Serial.println("started panel init");
  panel.resolution = 8;
  panel.init();
  panel.joystick_x0 = 128;
  panel.joystick_y0 = 128;
  panel.joystick_z0 = 128;
  


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
    //
    speed = map(panel.analog_in_state[4],0,255,-1000.0,1000.0);
  }

  if (panel.joystick_button.pressed() & (panel.joystick_button.currentDuration() > 250)  )
  {
    //Long joystick button press toggles color transformation enable
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
    matrix_from_xy(panel.joystick_x,panel.joystick_y);

    fan.compute_strobe_period(panel.analog_in_state[5],speed);
    drip.compute_strobe_period(127,speed);
    dance.compute_strobe_period(127,speed);

    //Interrupts off
    cli();

    fan.update_strobe_period();
    fan.set_transform_matrix(transform_matrix);
    drip.update_strobe_period();
    drip.set_transform_matrix(transform_matrix);
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