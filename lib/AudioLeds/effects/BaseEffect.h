#ifndef BASE_EFFECT_H
#define BASE_EFFECT_H

#include "AbstractEffect.h"
#include <FastLED.h>

class BaseEffect : public AbstractEffect
{
protected:
    virtual void clean();
    virtual void updateInput();
    virtual void fill();

    uint8_t getFillValue();

public:
    void setInputValue(uint8_t inputValue);
    virtual void loop() override;
};

#endif