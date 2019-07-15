#ifndef FIRE_EFFECT_H
#define FIRE_EFFECT_H

#include "BaseVUEffect.h"

#include <Arduino.h>

class FireEffect : public BaseVUEffect
{
private:
    bool gReverseDirection = false;
    CRGBPalette16 gPal;

    void Fire2012WithPalette();

protected:
    virtual void fill() override;

public:
    FireEffect();
};

#endif