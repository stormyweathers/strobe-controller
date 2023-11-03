class strobe_channel{
  public:
    float fundamental_freq_hz;
    uint16_t numerator;
    uint16_t denominator;

    volatile uint32_t strobe_period_us;
    
    volatile uint32_t *pulse_sequence_ptr;
    uint16_t pulse_sequence_size;

    volatile uint32_t pulse_code;
    volatile uint32_t pulse_count;

    float pulse_width_multiple;


    static uint8_t num_subchannels;

    // Pin numbers for each subchannel
    uint8_t* pin_list;
    // Timers for individual subchannel pulse widths
    OneShotTimer* pulse_width_timer_list;

    // Periodic timer for clocking intervals between pulses
    PeriodicTimer strobe_timer(TCK);


    //constructor
    strobe_channel(static uint8_t number_of_subchannels, int pin_numbers[], int pulse_timer_id )
    {
      // number_of_subchannels = 1,2,3, or 4
      // pulse_timer_id = TMR1, TMR2, or TMR3

      num_subchannels = number_of_subchannels;

      //allocate arrays for list of pins and list of oneshottimers
      pin_list = (uint8_t*)malloc(num_subchannels*sizeof(uint8_t));
      pulse_width_timer_list = (OneShotTimer*)malloc(num_subchannels*sizeof(OneShotTimer));

      //Default values to prevent things from crashing
      fundamental_freq = 60.0;
      numerator = 1;
      denominator = 1;
      pulse_code = 0xFFFFFFFF;

      //Initial values
      pulse_count = 0;

      for (int i=0; i< num_subchannels;i++)
      {
        // construct the pulse_width_timers
        pulse_width_timer_list[i] = OneShotTimer(pulse_timer_id);

        // Save the pin numbers for later
        pin_list[i] = pin_numbers[i];
      }
    }

    void initialize_timers()
    {
      //Start main timer in the off-state
      strobe_timer.begin(single_pulse,strobe_period_us, false);

      for (int i =0; i < num_subchannels; i++){
        // pulse width timer callback: Turn off the light for that subchannel
        pulse_width_timer_list[i].begin([] { digitalWriteFast(pin_list[i], LOW); });
      }
    }

  
    void single_pulse(){

      // Get the next pulse code
      pulse_code = *(pulse_sequence_ptr+pulse_count%pulse_sequence_size);

      // Send a pulse for to each subchannel determined by the pulse code
      for (int i =0; i < num_subchannels; i++){
       digitalWriteFast(pin_list[i],HIGH);
       pulse_width_timer_list[i].trigger(int(pulse_width_multiple*extract_pulse_code(pulse_code,i)) );
      }
      pulse_count++;
    }


    private:
      uint8_t extract_pulse_code(uint32_t full_pulse_code, uint8_t position)
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
        return ( static_cast<uint8_t>(  (BITS & 0xFF000000) >> (8*position)  );
      }

}