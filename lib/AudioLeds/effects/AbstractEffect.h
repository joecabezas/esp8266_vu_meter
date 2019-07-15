#ifndef ABSTACT_EFFECT_H
#define ABSTACT_EFFECT_H

#include <Arduino.h>

class AbstractEffect
{
    protected:
        float input;
    public:
        virtual void setInputValue(uint8_t inputValue);
        virtual void loop();
};

#endif