#ifndef FLICKERMATCH_H
#define FLICKERMATCH_H

/*
 * This file contains the code used for tuning the relative pulsewidths
 * of an RGB channel to minimize the visible flicket
 * This allows normalizing the apparent brightness of different color channels
 * This is necessary to minimize flicker when rapidly cycling colors
 * I dont forsee there being any reason to instantiate multiple instances of this
 * so I'll keep it as a namespace instead of a class
*/

//#include "globals.h"
#include "strobe_channel.h"
#include <ControlPanel.h>
#include "pulse_sequences.h"

class flicker_match{
    public:
        // Contains the LED pins and timers that we need here
        strobe_channel* channel_ptr;

        // Contains the analog input, buttons, screen
        controlPanel* panel_ptr;

        // Which analog inputs are used to tune the scalefactors?
        // 0-7 are technically available
        // 0-2 are the joystick XYZ

        uint8_t tuning_all = 3;
        uint8_t tuning_g = 4;
        uint8_t tuning_b = 5;

        float relative_width_factors[3] = {1.0,1.0,1.0};
        float overall_scale_factor = 1.0;

        float relative_min = 0.2;
        float relative_max = 5.0;

        volatile uint32_t *pulse_seq_lists[3] = {&R_G[0], &R_B[0], &R_G_B[0]};
        uint8_t pulse_seq_size_list[3] = {2,2,3}; 

        bool* update_display_ptr;

        // constructor
        flicker_match(bool* update_display_ptr)
        {
            this-> update_display_ptr = update_display_ptr;
        }

        void initialize(strobe_channel* channel_ptr, controlPanel* panel_ptr)
        {
            this->channel_ptr = channel_ptr;
            this->panel_ptr =  panel_ptr;
            this->tuning_g = tuning_g;
            this->tuning_b = tuning_b;
            // Set the channel parameters
            this->channel_ptr->pulse_sequence_ptr = pulse_seq_lists[0];
            this->channel_ptr->pulse_sequence_size = pulse_seq_size_list[0];
            this->channel_ptr->fundamental_freq_hz = 40;
            this->channel_ptr->numerator=1;
            this->channel_ptr->denominator =1;
            this->channel_ptr->compute_strobe_period(128,0);
            this->channel_ptr->update_strobe_period();
        }

        void run_match(strobe_channel* channel_ptr, controlPanel* panel_ptr){
            this->initialize(channel_ptr, panel_ptr);

            // how long to allow between singel clicles to read as double click
            uint16_t double_click_interval_ms = 250;
            uint8_t cycle_count = 0;

            while(true){
                
                this->panel_ptr->readState();

                // update color channel pulse_seqscale factors
                this->relative_width_factors[1] = map(float(this->panel_ptr->analog_in_state[this->tuning_g]),0.0,255.0,0.5,1.5);
                this->relative_width_factors[2] = map(float(this->panel_ptr->analog_in_state[this->tuning_b]),0.0,255.0,1,2);
                this->overall_scale_factor = map(float(this->panel_ptr->analog_in_state[this->tuning_all]),0.0,255.0,0.03125,32.0);
                this->channel_ptr->compute_strobe_period(128,0);

                cli();
                this->channel_ptr->relative_width_factors[0] = this->relative_width_factors[0];
                this->channel_ptr->relative_width_factors[1] = this->relative_width_factors[1];
                this->channel_ptr->relative_width_factors[2] = this->relative_width_factors[2];
                this->channel_ptr->pulse_width_multiple = this->overall_scale_factor;
                this->channel_ptr->update_strobe_period();
                sei();
                
                // Detect Single click to permute color comparisons for tuning
                if (this->panel_ptr->button.fell() && (this->panel_ptr->button.previousDuration() > double_click_interval_ms))
                {
                    cycle_count = (cycle_count+1)%3;
                    this->channel_ptr->pulse_sequence_ptr = this->pulse_seq_lists[cycle_count];
                    this->channel_ptr->pulse_sequence_size = this->pulse_seq_size_list[cycle_count];
                    //Serial.println("Single click detected:");


                }

                // Detect double click of panel.button to break loop
                //panel.button
                else if ( this->panel_ptr->button.pressed() && ( this->panel_ptr->button.previousDuration() < double_click_interval_ms) )
                {
                    // Double click detected
                    //Serial.println("double click detected");
                    return;
                }

                if (*update_display_ptr){
                    this->update_display();
                }
  
            }


        }

    private:
    void update_display()
    {
        this->panel_ptr->display.clearDisplay();
        this->panel_ptr->display.setCursor(0,0);

        this->panel_ptr->display.println("Flicker Match Calibration Mode");
        this->panel_ptr->display.println("------frequency------");
        this->panel_ptr->display.printf(" %6.2f Hz\n",channel_ptr->fundamental_freq_hz);
        this->panel_ptr->display.println("----scale factors----");
        this->panel_ptr->display.printf("R: %4.2f\n",this->relative_width_factors[0]);
        this->panel_ptr->display.printf("G: %4.2f\n",this->relative_width_factors[1]);
        this->panel_ptr->display.printf("B: %4.2f\n",this->relative_width_factors[2]);
        this->panel_ptr->display.println("----global width-----");
        this->panel_ptr->display.printf("   %4.2f",this->overall_scale_factor);
        this->panel_ptr->display.display();
    }

};

#endif