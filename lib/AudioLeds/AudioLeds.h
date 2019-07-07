#ifndef AUDIOLEDS_H
#define AUDIOLEDS_H

#include <vector>

#include <FastLED.h>

#include "effects/AbstractEffect.h"

class AudioLeds
{
private:
    uint16_t num_leds;
    CRGB *leds;

    std::vector<AbstractEffect *> effects;
    uint currentEffectIndex;

public:
    AudioLeds();

    void addEffect(AbstractEffect *effect);
    void nextEffect();
    void loop(uint8_t fill_size);

    AbstractEffect *getCurrentEffect();
};

#endif