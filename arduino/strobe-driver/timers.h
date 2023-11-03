#ifndef TIMERS_H
#define TIMERS_H

//Handles pulse widths of individual LED channels
OneShotTimer timer_fan_r(TMR1);
OneShotTimer timer_fan_g(TMR1);
OneShotTimer timer_fan_b(TMR1);
//OneShotTimer timer_w(TMR1);

OneShotTimer timer_dance_1(TMR2);
OneShotTimer timer_dance_2(TMR2);
OneShotTimer timer_dance_3(TMR2);
OneShotTimer timer_dance_4(TMR2);

OneShotTimer timer_drip_r(TMR3);
OneShotTimer timer_drip_g(TMR3);
OneShotTimer timer_drip_b(TMR3);


//Handles master strobe frequency
PeriodicTimer strobe_timer_fan(TCK);
PeriodicTimer strobe_timer_dance(TCK);
PeriodicTimer strobe_timer_drip(TCK);


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

void single_pulse_fan(){
  pulse_code_fan = *(pulse_sequence_ptr_fan+pulse_count_fan%pulse_sequence_size_fan);
  //pulse_code = apply_transform(pulse_code);

  digitalWriteFast(LED_FAN_R,HIGH);
  timer_fan_r.trigger(int( pulse_width_multiple*WIDTH_R(pulse_code_fan)) );

  digitalWriteFast(LED_FAN_G,HIGH);
  timer_fan_g.trigger(int( pulse_width_multiple*WIDTH_G(pulse_code_fan)) );

  digitalWriteFast(LED_FAN_B,HIGH);
  timer_fan_b.trigger(int( pulse_width_multiple*WIDTH_B(pulse_code_fan)) );
  pulse_count_fan++;
}

void single_pulse_dance(){
  pulse_code_dance = *(pulse_sequence_ptr_dance+pulse_count_dance%pulse_sequence_size_dance);
  //pulse_code = apply_transform(pulse_code);

  digitalWriteFast(LED_DANCE_1,HIGH);
  timer_dance_1.trigger(int( pulse_width_multiple*WIDTH_R(pulse_code_dance)) );

  digitalWriteFast(LED_DANCE_2,HIGH);
  timer_dance_2.trigger(int( pulse_width_multiple*WIDTH_G(pulse_code_dance)) );

  digitalWriteFast(LED_DANCE_3,HIGH);
  timer_dance_3.trigger(int( pulse_width_multiple*WIDTH_B(pulse_code_dance)) );

  digitalWriteFast(LED_DANCE_4,HIGH);
  timer_dance_4.trigger(int( pulse_width_multiple*WIDTH_W(pulse_code_dance)) );
  pulse_count_dance++;
}

void single_pulse_drip(){
  pulse_code_drip = *(pulse_sequence_ptr_drip+pulse_count_drip%pulse_sequence_size_drip);
  //pulse_code = apply_transform(pulse_code);

  digitalWriteFast(LED_DRIP_R,HIGH);
  timer_drip_r.trigger(int( pulse_width_multiple*WIDTH_R(pulse_code_drip)) );

  digitalWriteFast(LED_DRIP_G,HIGH);
  timer_drip_g.trigger(int( pulse_width_multiple*WIDTH_G(pulse_code_drip)) );
  
  digitalWriteFast(LED_DRIP_B,HIGH);
  timer_drip_b.trigger(int( pulse_width_multiple*WIDTH_B(pulse_code_drip)) );
  pulse_count_drip++;
}



void init_timers(){
  // begin timer in stopped mode
  strobe_timer_fan.begin(single_pulse_fan,strobe_period_us_fan, false);
  strobe_timer_dance.begin(single_pulse_dance,strobe_period_us_dance, false);
  strobe_timer_drip.begin(single_pulse_drip,strobe_period_us_drip, false);

  timer_fan_r.begin([] { digitalWriteFast(LED_FAN_R, LOW); });
  timer_fan_g.begin([] { digitalWriteFast(LED_FAN_G, LOW); });
  timer_fan_b.begin([] { digitalWriteFast(LED_FAN_B, LOW); });
  //timer_w.begin([] { digitalWriteFast(LED_W, LOW); });

  timer_dance_1.begin([] { digitalWriteFast(LED_DANCE_1, LOW); });
  timer_dance_2.begin([] { digitalWriteFast(LED_DANCE_2, LOW); });
  timer_dance_3.begin([] { digitalWriteFast(LED_DANCE_3, LOW); });
  timer_dance_4.begin([] { digitalWriteFast(LED_DANCE_4, LOW); });

  timer_drip_r.begin([] { digitalWriteFast(LED_DRIP_R, LOW); });
  timer_drip_g.begin([] { digitalWriteFast(LED_DRIP_G, LOW); });
  timer_drip_b.begin([] { digitalWriteFast(LED_DRIP_B, LOW); });

  //Read the panel inputs every 1ms
  //This frequency is necessary for the encoder to register pulses
  read_timer.begin(read_timer_callback,1000);

  display_timer.begin(display_timer_callback,100000);

}

#endif