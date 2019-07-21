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

        //this method is called before switching
        //to another effect, so implement this if
        //you need to do cleaning stuff like memory
        //cleaning
        virtual void sleep(){};
};

#endif