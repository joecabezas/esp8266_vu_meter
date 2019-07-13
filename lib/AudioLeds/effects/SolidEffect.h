#ifndef SOLID_EFFECT_H
#define SOLID_EFFECT_H

#include <FastLED.h>
#include "AbstractEffect.h"

class SolidEffect: public AbstractEffect
{
    private:
        float fillValue;
    public:
        virtual void setFillValue(uint8_t fillValue) override;
        virtual void loop() override;
};

#endif