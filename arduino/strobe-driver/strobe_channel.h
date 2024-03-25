
#ifndef STROBE_CHANNEL_H
#define STROBE_CHANNEL_H

#include "ITimerChannel.h"
#include <TeensyTimerTool.h>
//using namespace TeensyTimerTool;

#include "rhythm.h"
#include "transformations.h"

class strobe_channel{
  public:
    float fundamental_freq_hz;
    uint16_t numerator;
    uint16_t denominator;
    float speed_offset;
    
    float freq_hz;
    volatile uint32_t strobe_period_us;
    
    volatile uint32_t *pulse_sequence_ptr;
    uint16_t pulse_sequence_size;

    volatile uint32_t pulse_code;
    uint8_t  extracted_pulse_code[4];
    volatile uint32_t pulse_count;

    float pulse_width_multiple;


    uint8_t num_subchannels;

    // Pin numbers for each subchannel
    uint8_t* pin_list;
    // Timers for individual subchannel pulse widths
    TeensyTimerTool::OneShotTimer* pulse_width_timer_list;

    // Periodic timer for clocking intervals between pulses
    TeensyTimerTool::PeriodicTimer strobe_timer;

    bool manual_control = true;
    
    float speed_control_range_hz = 3.0;
    float slider_control_range_hz = 1.0;

    float rhythm_slope = 1;

    bool transform_enabled = 0;
    float transform_matrix[3][3]={{1,0,0},{0,1,0},{0,0,1}};
    float hue_param = 0.0;

    //constructor
    strobe_channel(uint8_t num_subchannels, int pin_numbers[],TeensyTimerTool::TimerGenerator* strobe_timer_id ,  TeensyTimerTool::TimerGenerator* pulse_timer_id )
    {
      // number_of_subchannels = 1,2,3, or 4
      // pulse_timer_id = TMR1, TMR2, or TMR3

      this->num_subchannels = num_subchannels;

      //allocate arrays for list of pins and list of oneshottimers
      this->pin_list = (uint8_t*)malloc(this->num_subchannels*sizeof(uint8_t));
      this->pulse_width_timer_list = (TeensyTimerTool::OneShotTimer*)malloc(this->num_subchannels*sizeof(TeensyTimerTool::OneShotTimer));

      //Default values to prevent things from crashing
      this->fundamental_freq_hz = 60.0;
      this->numerator = 1;
      this->denominator = 1;
      this->pulse_code = 0xFFFFFFFF;

      this->compute_strobe_period(0,0);

      //Initial values
      this->pulse_count = 0;

      this->strobe_timer = TeensyTimerTool::PeriodicTimer(strobe_timer_id);

      for (int i=0; i< this->num_subchannels;i++)
      {
        // construct the pulse_width_timers
        this->pulse_width_timer_list[i] = TeensyTimerTool::OneShotTimer(pulse_timer_id);

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
      if (this->transform_enabled)
      {
        this->pulse_code = this->apply_transform(this->transform_matrix,this->hue_param);
      }

      if ( float_rhythm(this->pulse_count, this->rhythm_slope) )
      {
        // Send a pulse for to each subchannel determined by the pulse code
        for (int i =0; i < this->num_subchannels; i++){
         digitalWriteFast(this->pin_list[i],HIGH);
         this->pulse_width_timer_list[i].trigger(int(this->pulse_width_multiple*this->extract_pulse_code(i)) );
        }
      }
      this->pulse_count++;
    }

    void compute_strobe_period(uint16_t slider_position,int16_t speed)
    {
      //Add offsets for fine-tuning and lever-controlled speed adjustmnet
      this->speed_offset = - map(float(speed),float(-1000),float(1000), -1*this->speed_control_range_hz,this->speed_control_range_hz);
      float slider_offset =  map(float(slider_position),0.0,255.0,-1*this->slider_control_range_hz,this->slider_control_range_hz);
      this->freq_hz = this->fundamental_freq_hz + this->speed_offset + slider_offset ;
      //apply ratio
      this->freq_hz = this->freq_hz * float(this->numerator)/float(this->denominator);
      this->strobe_period_us = static_cast<uint32_t> (1000000/this->freq_hz);
    }

    // Should always be called between sei(); / cli();
    void update_strobe_period()
    {
      this->strobe_timer.setPeriod(this->strobe_period_us);
    }

    uint32_t apply_transform(float transform_matrix[3][3], float hue_param)
    {
      float input_rgb[3];
      float output_rgb[3];
      float hue_step[3] = {0.0,0.0,0.0};
      uint8_t normalized_rgb[3];

      colorspace_operations::extract_pulse_code(this->pulse_code, input_rgb);
      colorspace_operations::compute_hue_delta(input_rgb, hue_param, hue_step);

      // apply hue step
      for (int i=0; i<3;i++)
      {
          input_rgb[i] += hue_step[i];
      }

      // Apply transform matrix
      colorspace_operations::matmul(output_rgb,transform_matrix,input_rgb);

      colorspace_operations::normalize_rgb(output_rgb, normalized_rgb);
      //use the bitmask to return a pulse normalized_rgbcode
      uint32_t new_pulse_code = colorspace_operations::reconstruct_pulse_code(normalized_rgb);
      return  new_pulse_code;
    }

    void set_transform_matrix(float mat[3][3])
    {
      for (int i=0;i<3;i++)
      {
        for (int j=0;j<3;j++)
        {
          this->transform_matrix[i][j] = mat[i][j];
        }
      }
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