#ifndef TIMERS_H
#define TIMERS_H

//Encoder interface
PeriodicTimer read_timer(TCK);
void read_timer_callback(){
  panel.enc.tick();
}

//Handles pulse widths of individual LED channels
OneShotTimer timer_r(TMR1);
OneShotTimer timer_g(TMR1);
OneShotTimer timer_b(TMR1);
OneShotTimer timer_w(TMR1);

OneShotTimer timer_1(TMR2);
OneShotTimer timer_2(TMR2);
OneShotTimer timer_3(TMR2);
OneShotTimer timer_4(TMR2);

//Handles master strobe frequency
PeriodicTimer strobe_timer(TCK);
PeriodicTimer strobe_timer_2(TCK);


PeriodicTimer display_timer(TCK);
bool update_display_flag = false;
void display_timer_callback(){
  update_display_flag = true;
}

#endif