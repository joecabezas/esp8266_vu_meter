#ifndef RAINBOW_EFFECT_H
#define RAINBOW_EFFECT_H

#include <FastLED.h>
#include "AbstractEffect.h"

class RainbowEffect: public AbstractEffect
{
    private:
        uint8_t hue;
    public:
        virtual void setInputValue(uint8_t inputValue) override;
        virtual void loop() override;
};

#endif