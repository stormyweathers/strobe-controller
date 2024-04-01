#ifndef TIMERS_H
#define TIMERS_H

//Encoder interface
TeensyTimerTool::PeriodicTimer read_timer(TeensyTimerTool::TCK);
void read_timer_callback(){
  panel.enc.tick();
}

TeensyTimerTool::PeriodicTimer display_timer(TeensyTimerTool::TCK);

void display_timer_callback(){
  update_display_flag = true;
}

void init_timers(){

  //Read the panel inputs every 1ms
  //This frequency is necessary for the encoder to register pulses
  read_timer.begin(read_timer_callback,1000);

  display_timer.begin(display_timer_callback, 100'000);

}

#endif