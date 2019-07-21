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
    AbstractEffect *currentEffect;
    uint8_t currentEffectIndex;

public:
    AudioLeds();


    void addEffect(AbstractEffect *effect);
    void previousEffect();
    void nextEffect();
    void setInputValue(uint8_t inputValue);
    void loop();
};

#endif