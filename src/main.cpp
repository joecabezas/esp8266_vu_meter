#include <Arduino.h>
#include <i2s.h>

#define SAMPLES 128 // make it a power of two for best DMA performance

int16_t getMicValue(){
  // read a sample
  int16_t l, r;
  i2s_read_sample(&l, &r, true);
  return l;
}

int getVolume(){
    // read a bunch of samples:
  int samples[SAMPLES];
 
  for (int i=0; i<SAMPLES; i++) {
    samples[i] = getMicValue();
  }
 
  // ok we have the samples, get the mean (avg)
  float meanval = 0;
  for (int i=0; i<SAMPLES; i++) {
    meanval += samples[i];
  }
  meanval /= SAMPLES;
  //Serial.print("# average: " ); Serial.println(meanval);
 
  // subtract it from all sapmles to get a 'normalized' output
  for (int i=0; i<SAMPLES; i++) {
    samples[i] -= meanval;
    //Serial.println(samples[i]);
  }
 
  // find the 'peak to peak' max
  float maxsample, minsample;
  minsample = 100000;
  maxsample = -100000;
  for (int i=0; i<SAMPLES; i++) {
    minsample = _min(minsample, samples[i]);
    maxsample = _max(maxsample, samples[i]);
  }

  return (maxsample - minsample);
}

void setup() {
  // Open serial communications and wait for port to open:
  // A baud rate of 115200 is used instead of 9600 for a faster data rate
  // on non-native USB ports
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start I2S at 16 kHz with 32-bits per sample
  if (!i2s_rxtx_begin(true, false)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }

  i2s_set_rate(16000);
}
 
void loop() {
  Serial.println(getVolume());
}