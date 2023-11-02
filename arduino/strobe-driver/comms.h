/* This Header includes the code 
 * for communication between two
 * strobe controllers, as well as
 * between the strobe controller and the serial device
 * that sets the master speed
 *
 */



#ifndef COMMS_H
#define COMMS_H 

char read_buff[8];
const char delim = '|';
int16_t speed =  0;
uint8_t mode = 0;


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
      if (bytes_read ==3){
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
  mode = static_cast<uint8_t> ( buff[2]);
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



#endif