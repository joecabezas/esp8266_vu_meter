#ifndef ABSTACT_EFFECT_H
#define ABSTACT_EFFECT_H

class AbstractEffect
{
    protected:
        CRGB *leds;
    public:
        virtual void setFillValue(uint8_t fillValue);
        virtual void loop();
};

#endif