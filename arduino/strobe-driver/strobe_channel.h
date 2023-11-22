
#ifndef STROBE_CHANNEL_H
#define STROBE_CHANNEL_H

#include "ITimerChannel.h"
#include <TeensyTimerTool.h>
//using namespace TeensyTimerTool;

#include "rhythm.h"
//#include "transformations.h"

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
    
    float speed_control_range_hz = 3.0;
    float slider_control_range_hz = 1.0;

    float rhythm_slope = 1;

    bool transform_enabled = 0;
    float transform_matrix[3][3]={{1,0,0},{0,1,0},{0,0,1}};

    //constructor
    strobe_channel(uint8_t num_subchannels, int pin_numbers[],TeensyTimerTool::TimerGenerator* strobe_timer_id ,  TeensyTimerTool::TimerGenerator* pulse_timer_id )
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

      this->strobe_timer = PeriodicTimer(strobe_timer_id);

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
      if (this->transform_enabled)
      {
        this->pulse_code = apply_transform(this->transform_matrix);
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

    uint32_t apply_transform(float transform_matrix[3][3])
    {
      float input_rgb[3];
      float output_rgb[3];
      uint8_t normalized_rgb[3];

      //Extract pulse code
      for (int i=0; i<3; i++)
      {
        input_rgb[i] = float(this->extract_pulse_code(i));
      }

      // Apply transform matrix
      this->matmul(output_rgb,transform_matrix,input_rgb);

      //Renormalize results back to ints
      //The new num may not be exactly 255, it may be off by 1 thats ok
      // Using absolute value is an artistic choice to get reflecting boundary conditions on the simplex
     float rescale = 255/(output_rgb[0] + output_rgb[1]+output_rgb[2]);
     for (int i =0; i<3; i++)
     {
      normalized_rgb[i]=static_cast<uint8_t> round(rescale*abs(output_rgb[i]));
      }

     //use the bitmask to return a pulse code
     uint32_t new_pulse_code = reconstruct_pulse_code(normalized_rgb);
//     Serial.printf(" in_rgb: %f,%f,%f::: out_rgb: %f,%f,%f  ::: normd: %i,%i,%i :: new_pulse_code = %#010x \n",
//                          input_rgb[0],input_rgb[1],input_rgb[2],output_rgb[0],output_rgb[1],output_rgb[2],normalized_rgb[0],normalized_rgb[1],normalized_rgb[2],new_pulse_code);
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
      void matmul(float out[3],float left[3][3], float right[3])
      {
        // 3x3 matrix multuplication
        for (uint8_t ii=0; ii<3;ii++){
          out[ii] = 0.0;
          for (uint8_t kk=0; kk<3; kk++)
          {
            out[ii] += left[ii][kk] * right[kk];
          }
        }
      }
      uint32_t reconstruct_pulse_code(uint8_t rgb[3])
      {
        uint32_t r_shifted = rgb[0] << 24;
        uint32_t g_shifted = rgb[1] << 16;
        uint32_t b_shifted = rgb[2] << 8;
        return r_shifted+g_shifted+b_shifted;
      }

      uint32_t reconstruct_pulse_code_4(uint8_t udlr[4])
      {
        uint32_t u_shifted = udlr[0] << 24;
        uint32_t d_shifted = udlr[1] << 16;
        uint32_t l_shifted = udlr[2] << 8;
        uint32_t r_shifted = udlr[3] << 0;
        return u_shifted+d_shifted+l_shifted+r_shifted;
      }

};

#endif