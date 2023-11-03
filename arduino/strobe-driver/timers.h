#ifndef TIMERS_H
#define TIMERS_H

//Encoder interface
PeriodicTimer read_timer(TCK);
void read_timer_callback(){
  panel.enc.tick();
}

PeriodicTimer display_timer(TCK);
bool update_display_flag = false;
void display_timer_callback(){
  update_display_flag = true;
}

void init_timers(){

  //Read the panel inputs every 1ms
  //This frequency is necessary for the encoder to register pulses
  read_timer.begin(read_timer_callback,1000);

  display_timer.begin(display_timer_callback,100000);

}

#endif