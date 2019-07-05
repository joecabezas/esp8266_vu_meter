#ifndef RAINBOW_EFFECT_H
#define RAINBOW_EFFECT_H

#include <Arduino.h>
#include <FastLED.h>
#include "AbstractEffect.h"

class RainbowEffect: public AbstractEffect
{
    private:
        uint8_t hue;
    public:
        virtual void loop(uint16_t fill_size) override;
};

#endif