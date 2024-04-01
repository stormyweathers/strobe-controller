#include <digitalWriteFast.h>
#include <TeensyTimerTool.h>
//using namespace TeensyTimerTool;
#include <math.h>
#include <Bounce2.h>
#include <elapsedMillis.h>

//my custom libs
#include <ControlPanel.h>
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
TeensyTimerTool::PeriodicTimer PulsePeriodTimer(TeensyTimerTool::TMR4);
uint32_t tick_period_us = 100;
uint32_t strobe_period_us = tick_period_us * 1000 ;

void TickTimerCallback(){
  train_r.ClockTick();
  train_g.ClockTick();
  train_b.ClockTick();
}
void SchedulePulseCallback(){
  train_r.AddPulse(250,300);
  train_g.AddPulse(500,300);
  train_b.AddPulse(750,300);
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
    TickTimer.begin(TickTimerCallback,tick_period_us);
    PulsePeriodTimer.begin(SchedulePulseCallback,strobe_period_us);
  }
  
  if (panel.button.fell() ){

    train_r.AddPulse(250,1000);
    train_g.AddPulse(500,1000);
    train_b.AddPulse(750,1000);

    train_r.PrintList();

  }
  

    //Interrupts off
    cli();
    

    //Interrupts on
    sei();


  if (update_display_flag){
    update_display();
  } 

}