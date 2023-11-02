const uint16_t RHYTHM_BUFF_SIZE=1024;
bool rhythm_buff[RHYTHM_BUFF_SIZE];
uint16_t rhythm_size = 10;
//Golden ratio
const float phi = 1.618033988749895;


bool fibonacci_rhythm(uint32_t n){
  /*
  compute the nth digit of the fibonacci quasicrystal word
  result is always a 1 or 0
  */
  return floor(n/phi) - floor((n-1)/phi)  ;
}

void euclidean_rhythm(uint16_t hits, uint16_t steps){
  /*
  precompute the buffer necessary for a euclidean rhythm
  access the buff later using: rhythm_buff[i%rhythm_size]
  */
  for (uint16_t i =0; i<steps; i++){
    rhythm_buff[i] = floor(i*hits/steps) - floor((i-1)*hits/steps);
  }
  rhythm_size = steps;
}

bool float_rhythm(uint32_t n, float slope){
  // slope must be < 1 !!
  return floor(n*slope) - floor((n-1)*slope)  ;
}