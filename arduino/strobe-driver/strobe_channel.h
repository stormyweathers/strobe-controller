#include "ITimerChannel.h"
#ifndef STROBE_CHANNEL_H
#define STROBE_CHANNEL_H

#include <TeensyTimerTool.h>
//using namespace TeensyTimerTool;

class strobe_channel{
  public:
    float fundamental_freq_hz;
    uint16_t numerator;
    uint16_t denominator;
    
    float freq_hz;
    volatile uint32_t strobe_period_us;
    
    volatile uint32_t *pulse_sequence_ptr;
    uint16_t pulse_sequence_size;

    volatile uint32_t pulse_code;
    volatile uint32_t pulse_count;

    float pulse_width_multiple;


    uint8_t num_subchannels;

    // Pin numbers for each subchannel
    uint8_t* pin_list;
    // Timers for individual subchannel pulse widths
    OneShotTimer* pulse_width_timer_list;

    // Periodic timer for clocking intervals between pulses
    PeriodicTimer strobe_timer;

    bool manual_control = true;

    //constructor
    strobe_channel(uint8_t num_subchannels, int pin_numbers[], TeensyTimerTool::TimerGenerator* pulse_timer_id )
    {
      // number_of_subchannels = 1,2,3, or 4
      // pulse_timer_id = TMR1, TMR2, or TMR3

      this->num_subchannels = num_subchannels;

      //allocate arrays for list of pins and list of oneshottimers
      this->pin_list = (uint8_t*)malloc(this->num_subchannels*sizeof(uint8_t));
      this->pulse_width_timer_list = (OneShotTimer*)malloc(this->num_subchannels*sizeof(OneShotTimer));

      //Default values to prevent things from crashing
      this->fundamental_freq_hz = 60.0;
      this->numerator = 1;
      this->denominator = 1;
      this->pulse_code = 0xFFFFFFFF;

      this->compute_strobe_period(0,0);

      //Initial values
      this->pulse_count = 0;

      this->strobe_timer = PeriodicTimer(TCK);

      for (int i=0; i< this->num_subchannels;i++)
      {
        // construct the pulse_width_timers
        this->pulse_width_timer_list[i] = OneShotTimer(pulse_timer_id);

        // Save the pin numbers for later
        this->pin_list[i] = pin_numbers[i];
      }
    }

    TeensyTimerTool::errorCode initialize_timers()
    {

      TeensyTimerTool::errorCode err;
      //Start main timer in the off-state
      // use lambda with [this] capture to use nonstatic member function as callback
      err = this->strobe_timer.begin(  [this] {this->single_pulse();} ,this->strobe_period_us, false);

      for (int i =0; i < this->num_subchannels; i++){
        // pulse width timer callback: Turn off the light for that subchannel
        // [=] to capture local variables by value in lambda
        err = pulse_width_timer_list[i].begin( [=] { digitalWriteFast(this->pin_list[i], LOW); });
      }
      return err;
    }

  
    void single_pulse()
    {

      // Get the next pulse code
      this->pulse_code = *(this->pulse_sequence_ptr+this->pulse_count%this->pulse_sequence_size);

      // Send a pulse for to each subchannel determined by the pulse code
      for (int i =0; i < this->num_subchannels; i++){
       digitalWriteFast(this->pin_list[i],HIGH);
       this->pulse_width_timer_list[i].trigger(int(this->pulse_width_multiple*this->extract_pulse_code(i)) );
      }
      this->pulse_count++;
    }

    void compute_strobe_period(uint16_t slider_position,int16_t speed)
    {
      float offset_hz = map(slider_position,0,255,-5,5);
      if (manual_control)
      {
        this->freq_hz = this->fundamental_freq_hz*float(this->numerator)/float(this->denominator);
      }
      else
      {
        this->freq_hz = map(speed,-1000,1000,140+offset_hz,100-offset_hz);
      }
      this->strobe_period_us = static_cast<uint32_t> (1000000/this->freq_hz);
    }

    // Should always be called between sei(); / cli();
    void update_strobe_period()
    {
      this->strobe_timer.setPeriod(this->strobe_period_us);
    }


    private:
      uint8_t extract_pulse_code( uint8_t position)
      {
        /* Get the 8-bit pulse code from a full 4-channel 32-bit pulse code word
         *In hex:   0xRRGGBBWW
         * extract_pulse_code(0xRRGGBBWW,0) = 0xRR
         * extract_pulse_code(0xRRGGBBWW,1) = 0xGG
         * extract_pulse_code(0xRRGGBBWW,2) = 0xBB
         * extract_pulse_code(0xRRGGBBWW,3) = 0xWW 
         * extract_pulse_code(0xRRGGBBWW,>3)= 0x00
         */
        if (position > 3)
        {
          return 0;
        }
        position = 3-position;
        uint32_t mask = 0xFF << (8*position);
        return ( static_cast<uint8_t>(  ( this->pulse_code & mask ) >> (8*position)  ) );
      }

};

#endif