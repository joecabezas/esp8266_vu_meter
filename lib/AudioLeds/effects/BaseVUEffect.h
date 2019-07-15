#ifndef BASE_VU_EFFECT_H
#define BASE_VU_EFFECT_H

#include "AbstractEffect.h"
#include <FastLED.h>

class BaseVUEffect : public AbstractEffect
{
protected:
    virtual void fill();
    uint8_t getFillValue();

public:
    void setInputValue(uint8_t inputValue);
    virtual void loop() override;
};

#endif