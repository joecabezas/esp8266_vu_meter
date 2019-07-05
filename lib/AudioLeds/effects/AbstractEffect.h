#ifndef ABSTACT_EFFECT_H
#define ABSTACT_EFFECT_H

class AbstractEffect
{
    protected:
        CRGB *leds;
    public:
        virtual void loop(uint16_t fill_size);
};

#endif