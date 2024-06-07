#include <digitalWriteFast.h>
#include <TeensyTimerTool.h>
//using namespace TeensyTimerTool;
#include <math.h>
#include <Bounce2.h>
#include <elapsedMillis.h>

//my custom libs
#include <ControlPanel.h>
#include <easing.h>

//Local includes
#include "globals.h"
#include "pulse_sequences.h"
//#include "comms.h"
#include "menu.h"
#include "strobe_channel.h"
#include "timers.h"
#include "transformations.h"
#include "PulseTrain.h"
#include "flicker_match.h"


const std::vector<int32_t> init_data = {-10,10,-10,30,10,-10,10 };
const std::vector<int32_t> osc_data = {-1000,1000,-1000,1000,-1000,
                                             1000,-1000,1000,-1000,
                                             1000,-1000,1000,-1000,
                                             1000,-1000,1000,-1000,
                                             1000,-1000,1000,-1000,
                                             INT32_MIN,INT32_MIN };

PulseTrain train_r(&osc_data,4, false);
PulseTrain train_g(&osc_data,5, false);
PulseTrain train_b(&osc_data,3, false);

TeensyTimerTool::PeriodicTimer TickTimer(TeensyTimerTool::TMR4);
TeensyTimerTool::PeriodicTimer PulsePeriodTimer(TeensyTimerTool::GPT1);
uint32_t tick_period_us = 50;
uint32_t strobe_period_us = 100'000;
uint32_t strobe_timer_count = 0;
int32_t  offset_us =0;
double offset = 0;
double modulation_time = 0;
double modulation_frequency_hz = 0.5;
double modulation_period_us = 1e6/modulation_frequency_hz;

void TickTimerCallback(){
  train_r.ClockTick();
  train_g.ClockTick();
  train_b.ClockTick();
}
void SchedulePulseCallback(){
  modulation_time +=  strobe_period_us;
  offset =(strobe_period_us-tick_period_us*255*4)/2.0;

  train_r.AddPulse( (offset                      )/tick_period_us,255.0*4/tick_period_us);
  train_g.AddPulse( (offset+strobe_period_us*0.125*sin(TWO_PI*modulation_time/modulation_period_us))
                          /tick_period_us,255.0*4/tick_period_us);
  train_b.AddPulse( (offset+strobe_period_us*0.125*cos(TWO_PI*modulation_time/modulation_period_us))
                          /tick_period_us,255.0*4/tick_period_us);
}

void all_on(){
  PulsePeriodTimer.start();
  strobe_enabled = true;
}
void all_off(){
  PulsePeriodTimer.stop();
  strobe_enabled = false;
}


void setup() {
  //TeensyTimerTool error handler
  TeensyTimerTool::attachErrFunc(TeensyTimerTool::ErrorHandler(Serial));

  Wire1.begin();

  Serial.println("started panel init");
  panel.resolution = 8;
  panel.init(oled_orientation);
  panel.joystick_x0 = 123;
  panel.joystick_y0 = 126;
  panel.joystick_z0 = 123;
  //defined in menu.h
  panel.enc.attachButtonCallback(onButtonChanged);
  panel.enc.attachCallback(onRotorChanged);
  init_timers();


  strobe_enabled = 1;
  TickTimer.begin(TickTimerCallback,tick_period_us);
  PulsePeriodTimer.begin(SchedulePulseCallback,strobe_period_us,true);
  PulsePeriodTimer.start();

  Serial.println("initialized");

}

uint32_t modulation_period_ms = 5000;
elapsedMillis since_cycle;

void loop() {

  // Read human inputs
  panel.readState();
  
  // Double click main button to enter flicker match calibration mode
  if ( panel.button.pressed() && ( panel.button.previousDuration() < 250) )
  {
    
  }

  // Toggle is up (=0)
  if ( (!strobe_enabled) & !panel.toggle.read()) {
    Serial.println("Turning on");
    all_on();
    //PulsePeriodTimer.start();
    //TickTimer.start();
    //strobe_enabled = 1;
  }
  if ( (strobe_enabled) & panel.toggle.read()) {
    Serial.println("Turning off");
    all_off();
    //PulsePeriodTimer.stop();
    //TickTimer.stop();
    //strobe_enabled = 0;
  }
  // Double click main button to rotate oled
  if ( panel.joystick_button.pressed() && ( panel.joystick_button.previousDuration() < 250) )
  {
    oled_orientation = (oled_orientation+2)%4;
    panel.display.setRotation(oled_orientation);
    update_display();
  }

  
  if (panel.joystick_button.fell()){
    
    if(train_r.PerformTests()){
      Serial.println("The pulse train test passed!");
    }

  }
  
  if (panel.button.fell() ){

    train_r.AddPulse(250.0,1000.0);
    train_g.AddPulse(750.0,1000.0);
    train_b.AddPulse(1250.0,1000.0);

    //train_r.PrintList();

  }
  

    //Interrupts off
    cli();
    

    //Interrupts on
    sei();


  if (update_display_flag){
    update_display();
  } 



}