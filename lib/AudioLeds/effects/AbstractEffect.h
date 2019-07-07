#ifndef ABSTACT_EFFECT_H
#define ABSTACT_EFFECT_H

class AbstractEffect
{
    protected:
        CRGB *leds;
    public:
        virtual void loop(uint8_t fill_size);
};

#endif