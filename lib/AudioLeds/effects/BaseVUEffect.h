#ifndef BASE_VU_EFFECT_H
#define BASE_VU_EFFECT_H

#include "BaseEffect.h"
#include <FastLED.h>

class BaseVUEffect : public BaseEffect
{
protected:
    virtual void clean() override;
};

#endif