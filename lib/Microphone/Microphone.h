#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <Arduino.h>
#include <i2s.h>

class Microphone
{
public:
    Microphone(int sample_rate);

    int getSample();
    int getVolume(int audio_samples = 64);
};

#endif