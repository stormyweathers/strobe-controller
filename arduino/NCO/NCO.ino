#include <TeensyTimerTool.h>
#include "Teensy_PWM.h"
#include <Wire.h>



#define OUTPUT_PIN 13
#define I2C_ADDRESS 0xAF

Teensy_PWM* PWM_Instance;
const uint32_t write_resolution = 10;
const float pwm_freq = 50000.0;

const uint16_t num_waveform_samples = 1024;
uint16_t waveform_samples[num_waveform_samples];
float waveform_frequency = 1.0;


uint16_t sin_waveform(uint16_t sample_num)
{
  //Use different math here to generate the waveform samples
  // should always be normalized such that:
  //  Peak-Peak signal goes from 0 --> 2^write_resolution -1
  //  phase from 0-->num_waveform_samples is one full cycle

  float phase = sample_num * (TWO_PI/num_waveform_samples);
  float wave_sample = (sin(phase-PI/2)+1)/2;
  uint32_t amplitude = pow(2,write_resolution)-1 ;
  uint16_t quantized_wave = constrain( static_cast<uint32_t>(round(wave_sample * amplitude)) , amplitude*0 ,amplitude) ;
  return quantized_wave;
}
void compute_waveform_samples( uint16_t (*waveform_func) (uint16_t), uint16_t sample_buff[]  )
{
  for (int i =0; i< num_waveform_samples; i++)
  {
    sample_buff[i] = waveform_func(i);
  }
}

void print_buff( uint16_t buff[], uint16_t buff_size  )
{
  for (int i =0; i< buff_size; i++)
  {
    Serial.printf("idx %04i: %i\n", i, buff[i]);
  }
}

TeensyTimerTool::errorCode err;
TeensyTimerTool::PeriodicTimer waveform_timer(TeensyTimerTool::TCK);
volatile uint16_t sample_idx=0;
void waveform_timer_callback(){
  analogWrite(OUTPUT_PIN,waveform_samples[sample_idx]);
  sample_idx = (sample_idx+1)%num_waveform_samples;
}
uint32_t period_from_frequency(float frequency)
{
  //Timer periods are in us
  // Frequencies are in Hz
  //The static_casts are around because the round macro gives signed int type
  uint32_t full_waveform_period = static_cast<uint32_t>( round(1000000/frequency));
  uint32_t sample_period = static_cast<uint32_t>( round( full_waveform_period/ num_waveform_samples  ));
  return sample_period;
}


float new_waveform_frequency = 0;
void receiveEvent(int howMany)
{
  //Serial.printf("Recieved mess with %i bytes\n",howMany);
  new_waveform_frequency = parseMsg(); 
  //Serial.printf("Recoeved new waveform freq: %f\n",new_waveform_frequency);
}

float parseMsg()
{
  char buff[4];
  uint8_t bytesRead = 0;
  //while( Wire1.available() & (bytesRead < 4) )
  while(  (bytesRead < 4) )
  {
    buff[bytesRead++] = Wire1.read();
    //Serial.printf("%#x",buff[bytesRead-1]);
  }
  //Interpret the 4-bytes as a float
  return  *(float*)(buff) ;
}


void testByteParse(float number)
{
  union {
    float f;
    uint32_t u;
    } f2u = { .f = number };

  //Serial.printf("%f --> %#010X\n",number,f2u.u);

  unsigned char buff[4];
  for (int i=0;i<4; i++)
  {
    buff[i]= *(  (unsigned char *)(&number)+i ) ;
  }
  
  float reconstructed = *(float*)(buff);

  Serial.printf("%010.5f --> 0x%02X %02X %02X %02X --> %010.5f \n",number,(uint8_t) buff[0], (uint8_t)buff[1],(uint8_t)buff[2],(uint8_t)buff[3],reconstructed);

  if (number == reconstructed)
  {
    Serial.println("Equality checked!");
  }
  else
  {
    Serial.println("Equality failed!");
  }

}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  delay(500);

  Serial.println("Precomputing waveform");
  analogWriteResolution(write_resolution);
  analogWriteFrequency(OUTPUT_PIN,pwm_freq);
  compute_waveform_samples(&sin_waveform, waveform_samples);
  //print_buff(waveform_samples,num_waveform_samples);

  Serial.println("Initializing waveform timer");
  err = waveform_timer.begin(  waveform_timer_callback,period_from_frequency(waveform_frequency)  , true);

  Serial.printf("Initializing I2C comm with addr: %i \n",I2C_ADDRESS);
  Wire1.begin(I2C_ADDRESS);
  Wire1.onReceive(receiveEvent);

  //testByteParse(-10000*TWO_PI);

}

void loop() {
  // put your main code here, to run repeatedly:


  if (new_waveform_frequency > 0.1)
  {
    Serial.printf("Recieved new freq: %02.2f\n",new_waveform_frequency);
    waveform_timer.setPeriod( period_from_frequency(new_waveform_frequency) );
    //Serial.printf("Set freq to: %f \n",new_waveform_frequency);
    new_waveform_frequency = 0;
  }
}
