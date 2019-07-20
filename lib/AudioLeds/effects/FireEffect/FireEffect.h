#ifndef FIRE_EFFECT_H
#define FIRE_EFFECT_H

#include "effects/BaseEffect.h"

#include <Arduino.h>

class FireEffect : public AbstractEffect
{
private:
    uint32_t millisecondsPassed;
    bool gReverseDirection = false;
    CRGBPalette16 pallete;

    void Fire2012WithPalette();

protected:
    virtual void setInputValue(uint8_t inputValue) override;
    virtual void loop() override;

public:
    FireEffect();
    FireEffect(CRGBPalette16 _pallete);
};

#endif