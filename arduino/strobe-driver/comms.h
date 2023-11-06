/* This Header includes the code 
 * for communication between two
 * strobe controllers, as well as
 * between the strobe controller and the serial device
 * that sets the master speed
 *
 */
#ifndef COMMS_H
#define COMMS_H 

#include "pulse_sequences.h"
#include "globals.h"
#include "transformations.h"

char read_buff[8];
const char delim = '|';



//Size of message
uint8_t num_bytes = 5;

bool read_line(char buff[]){ 
  // Read a single line from serial buffer into read_buff
  uint8_t bytes_read = 0;
  char c = 0;
  while (Serial.available() ) {
    if (Serial.available() >0) {
      c = Serial.read();  //gets one byte from serial buffer
      //if (c == '\n'){
      if (c == '\n'){
        return 1;
      }
      buff[bytes_read++] = c; //makes the string readString
      if (bytes_read == num_bytes){
        return 1;
      }
    } 
  }
  return 0;
  Serial.flush();
}

void parse_line(char buff[]){
  // Message structure:
  // < SPEED LSB > <SPEED MSB> <MODE>
  Serial.println("PArsing msg!");
  speed = static_cast<int16_t> (  (  int(buff[1]) << 8) |  int(buff[0]) );
  color_mode = static_cast<uint8_t> ( buff[2]);
  freq_mode = static_cast<uint8_t> ( buff[3]);
  strobe_coin_enabled_prev = strobe_coin_enabled;
  strobe_coin_enabled = static_cast<bool>(buff[4]);
  Serial.flush();
}

// Define the list of mode codes
// 3 modes for the pulse seqeunce:
/* PULSE MODES
 * 1 --> 1 step, white
 * 2 --> 3 step, cycle RGB
 * 3 --> 9 step, sierpinski path
 */

 // Frequency MODES
/* Fan:
 * 
 *
 *
 *
 *
 */

void apply_mode_fan(uint8_t color_mode, uint8_t freq_mode)
{
  const uint16_t* numerators;
  const uint16_t* denominators;
  const float* speed_tuning_ranges;

  const volatile uint32_t* pulse_seqs;
  const uint32_t* pulse_sizes;
  
// could not find a way to cast pointer array in place like the other params
  volatile uint32_t* pulse_seqs_2[] = {&two_tone[0],&two_tone[0],&two_tone[0], &fractal_path_0[0],&fractal_path_0[0],&fractal_path_0[0], &four_tone[0], &five_tone[0]};
  const volatile uint32_t pulse_seq_sizes_2[] = {2,2,2,3,3,3,4,5};
  const float    transform_angles[] = {0,       5*PI/3,    PI/2,    PI/4,       PI,       0,   7*PI/4, PI/2};

  color_mode = constrain(color_mode,1,3);
  freq_mode = constrain(freq_mode,1,8);
  switch (color_mode)
  {
    case 1:
    // Slow color change, too slow for strobe effects
      fan.pulse_sequence_ptr = &fractal_path_4[0];
      fan.pulse_sequence_size = 243;
      /*
      3:1 (5p)
      6:1 (10p)
      9:1 (15p)
      21:2 (shimmering many)
      27:2 (shimmering, many+)
      39:2 (shimmering, many++)
      45:2 (many+++)
      83:2 (most)
      */
      numerators = (const uint16_t [] )      {3,  6,9,21,27,39,45,83};
      denominators = (const uint16_t [] )    {1,  1,1, 2, 2, 2, 2, 2};
      speed_tuning_ranges = (const float []) {3,  3,3,3,3,3,3,3 };

      break;
    
    case 2:
    // R-> G -> B Mode
      fan.pulse_sequence_ptr = pulse_seqs_2[freq_mode-1];
      fan.pulse_sequence_size = pulse_seq_sizes_2[freq_mode-1];

      rotation_matrix_full(transform_angles[freq_mode-1],fan.transform_matrix);
      fan.transform_enabled = true;
      /*
        6:1 ratio, two-tone 0deg (10 petal)
        6:1 ratio, two-tone 30deg (10 petal)
        6:1 ratio, two-tone -90deg (10 petal)

        3-3-3
        9:1 ratio, 3-tone -30deg (15 petal)
        9:1 ratio, 3-tone 30deg (15 petal)
        9:1 ratio, 3-tone 180deg (15 petal)


        4
        12:1 ratio, 4-tone (-30deg) (20 petal)

        5
        16:1 20 petals 90 deg
      */
      numerators = (const uint16_t [] )   {6,6,6,9,9,9,12,15};
      denominators = (const uint16_t [] ) {1,1,1,1,1,1,1,1};
      speed_tuning_ranges = (const float []) {3,3,3,3,3,3,3,3 };



      break;

    case 3:
    // 9-step fractal path
      fan.pulse_sequence_ptr = &fractal_path_1[0];
      fan.pulse_sequence_size = 9;
      numerators = (const uint16_t [] ){1,5,10,15,15,20,45,135};
      denominators =(const uint16_t [] ){1,1,1,1,4,1,1,1};
      break;

    default: 
      numerators = (const uint16_t [] ){1,1,1,1,1,1,1,1};
      denominators =(const uint16_t [] ){1,1,1,1,1,1,1,1};
      break;
  }

    fan.numerator = numerators[freq_mode-1];
    fan.denominator = denominators[freq_mode-1];
}

void apply_mode_drip(uint8_t color_mode, uint8_t freq_mode)
{
  const uint16_t* numerators;
  const uint16_t* denominators;
  const float* speed_tuning_ranges;
  color_mode = constrain(color_mode,1,3);
  freq_mode = constrain(freq_mode,1,8);
  switch (color_mode)
  {
    case 1:
    // Slow color change, too slow for strobe effects
      drip.pulse_sequence_ptr = &fractal_path_6[0];
      drip.pulse_sequence_size = 2187;
      numerators = (const uint16_t [] ){1,2,3,4,5,6,7,8};
      denominators = (const uint16_t [] ) {1,1,1,1,1,1,1,1};
      speed_tuning_ranges = (const float [] ) {10.0, 7.0, 6.0, 5.0, 4.8, 4.6, 3.3, 4.3 };
      break;
    
    case 2:
    // R-> G -> B Mode
    // TUNED!
      drip.pulse_sequence_ptr = &fractal_path_0[0];
      drip.pulse_sequence_size = 3;
      numerators = (const uint16_t [] ){1,4,2,8,3,4,5,6};
      denominators = (const uint16_t [] ) {1,3,1,3,1,1,1,1};
      speed_tuning_ranges = (const float [] ) {10.0, 2.5, 6.25, 2.0, 6.0,4.0,2.5,4.5};
      break;

    case 3:
    // 6-step star path
      drip.pulse_sequence_ptr = &six_step[0];
      drip.pulse_sequence_size = 6;
      //numerators = (const uint16_t [] ){1,4,3,2,3,4,5,6};
      //denominators = (const uint16_t [] ) {1,3,2,1,1,1,1,1};
      numerators = (const uint16_t [] ){1,3,3,4,5,6,4,2};
      denominators = (const uint16_t [] ) {1,2,1,1,1,1,3,1};
      speed_tuning_ranges = (const float [] ) {10.0, 5.0, 6.0, 8.0, 8.0, 4.4, 2.0 , 7};
      break;

    default: 
      numerators = (const uint16_t [] ){1,4,2,8,3,4,5,6};
      denominators = (const uint16_t [] ) {1,3,1,3,1,1,1,1};
      speed_tuning_ranges = (const float [] ) {10.0, 2.5, 6.25, 2.0, 6.0,4.0,2.5,4.5};
      break;
  }

    drip.numerator = numerators[freq_mode-1];
    drip.denominator = denominators[freq_mode-1];
    drip.speed_control_range_hz = speed_tuning_ranges[freq_mode-1];
}

#endif