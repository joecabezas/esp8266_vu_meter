#include "Microphone.h"

#define ERROR_FAILED_TO_INITIALIZE_I2S 1

Microphone::Microphone(int sample_rate)
{
    if (!i2s_rxtx_begin(true, false))
    {
#ifdef DEBUG
        Serial.println("Failed to initialize I2S!");
#endif
        throw ERROR_FAILED_TO_INITIALIZE_I2S;
    }

    i2s_set_rate(sample_rate);
}

int Microphone::getSample()
{
    int16_t l, r;
    i2s_read_sample(&l, &r, true);
    return l;
}

int Microphone::getVolume(int audio_samples)
{
    // read a bunch of samples:
    int samples[audio_samples];

    for (int i = 0; i < audio_samples; i++)
    {
        samples[i] = getSample();
    }

    // ok we have the samples, get the mean (avg)
    float meanval = 0;
    for (int i = 0; i < audio_samples; i++)
    {
        meanval += samples[i];
    }
    meanval /= audio_samples;

    // subtract it from all sapmles to get a 'normalized' output
    for (int i = 0; i < audio_samples; i++)
    {
        samples[i] -= meanval;
    }

    // find the 'peak to peak' max
    float maxsample, minsample;
    minsample = 100000;
    maxsample = -100000;
    for (int i = 0; i < audio_samples; i++)
    {
        minsample = _min(minsample, samples[i]);
        maxsample = _max(maxsample, samples[i]);
    }

    return maxsample - minsample;
}

uint8_t Microphone::get8BitVolume(int audio_samples)
{
    return _min(getVolume(audio_samples), 255);
}