#ifndef RAINBOW_VELOCITY_EFFECT_H
#define RAINBOW_VELOCITY_EFFECT_H

#include <FastLED.h>
#include "AbstractEffect.h"

// this is a local import in a local library
// dependency was added in library.json
#include "MathUtils.h"

class RainbowVelocityEffect: public AbstractEffect
{
    private:
        uint8_t hue;
    public:
        virtual void setInputValue(uint8_t inputValue) override;
        virtual void loop() override;
};

#endif