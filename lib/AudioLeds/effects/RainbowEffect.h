#ifndef RAINBOW_EFFECT_H
#define RAINBOW_EFFECT_H

#include <Arduino.h>
#include <FastLED.h>
#include "AbstractEffect.h"

class RainbowEffect: public AbstractEffect
{
    private:
        uint8_t hue;
        float fillValue;
    public:
        virtual void setFillValue(uint8_t fillValue) override;
        virtual void loop() override;
};

#endif