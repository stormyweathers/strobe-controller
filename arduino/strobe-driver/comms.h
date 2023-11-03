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

char read_buff[8];
const char delim = '|';
int16_t speed =  0;
uint8_t color_mode = 0;
uint8_t freq_mode = 0;
//Size of message
uint8_t num_bytes = 4;

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
  switch (color_mode)
  {
    case 0:
    // Slow color change, too slow for strobe effects
      pulse_sequence_ptr_fan = &fractal_path_4[0];
      pulse_sequence_size_fan = 243;

      break;
    
    case 1:
    // R-> G -> B Mode
      pulse_sequence_ptr_fan = &fractal_path_0[0];
      pulse_sequence_size_fan = 3;
      break;

    case 2:
    // 9-step fractal path
      pulse_sequence_ptr_fan = &fractal_path_1[0];
      pulse_sequence_size_fan = 9;

    default: 
      break;
  }

  uint16_t numerators[8] = {1,5,10,15,15,20,45,135};
  uint16_t denominators[8] = {1,1,1,1,4,1,1,1};
  

// 1:1 5:1 10:1 15:1 15:4 20:1 45:1 135:1

}



#endif