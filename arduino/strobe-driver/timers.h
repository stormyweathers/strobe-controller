#ifndef TIMERS_H
#define TIMERS_H

//Encoder interface
PeriodicTimer read_timer(TCK);
void read_timer_callback(){
  panel.enc.tick();
}

PeriodicTimer display_timer(TCK);
void display_timer_callback(){
  update_display_flag = true;
}

PeriodicTimer mode_timer(TCK64);
void mode_timer_callback(){

  /*
  //Increment the mode parameter
    freq_mode++;

    if ( 9 == freq_mode  )
    {
      color_mode = color_mode%3+2;
      freq_mode = 1;
    }
  */
    apply_mode_fan(&fan, random(1,4), random(1,9));
    apply_mode_spot(&spot, random(1,4), random(1,9));
}

void init_timers(){

  //Read the panel inputs every 1ms
  //This frequency is necessary for the encoder to register pulses
  read_timer.begin(read_timer_callback,1000);

  // Update display @ 10 Hz
  display_timer.begin(display_timer_callback, 100'000);

  //Increment Mode every 10s
  err = mode_timer.begin(mode_timer_callback, 10'000'000);

}

#endif