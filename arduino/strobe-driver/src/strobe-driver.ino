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
//#include "comms.h"
#include "menu.h"
#include "strobe_channel.h"
#include "timers.h"
#include "transformations.h"
#include "PulseTrain.h"
#include "flicker_match.h"




const std::vector<int32_t> init_data = {-10,10,-10,10};
PulseTrain train(&init_data, true);

void setup() {
  //TeensyTimerTool error handler
  attachErrFunc(ErrorHandler(Serial));

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
    //Joystick Cycles through modes, but skip color_mode ==1
    train.AddPulse(10,30);
    train.PrintList();
  }
  
  if (panel.button.fell() ){

  }
  

    //Interrupts off
    cli();
    

    //Interrupts on
    sei();


  if (update_display_flag){
    update_display();
  } 

}