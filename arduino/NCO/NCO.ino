#include <TeensyTimerTool.h>
#include "Teensy_PWM.h"


#define OUTPUT_PIN 13

Teensy_PWM* PWM_Instance;
const uint32_t write_resolution = 10;
const float pwm_freq = 50000.0;

const uint16_t num_waveform_samples = 1024;
uint16_t waveform_samples[num_waveform_samples];
float waveform_frequency = 10.0;


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


}

void loop() {
  // put your main code here, to run repeatedly:

}
