#ifndef RAINBOW_EFFECT_H
#define RAINBOW_EFFECT_H

#include "effects/BaseVUEffect.h"

class RainbowEffect : public BaseVUEffect
{
private:
    uint8_t hue;

protected:
    virtual void clean() override;

public:
    virtual void fill() override;
};

#endif