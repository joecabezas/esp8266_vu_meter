#ifndef RAINBOW_VELOCITY_EFFECT_H
#define RAINBOW_VELOCITY_EFFECT_H

#include "effects/BaseVUEffect.h"

// this is a local import in a local library
// dependency was added in library.json
#include "MathUtils.h"

class RainbowVelocityEffect: public BaseVUEffect
{
    private:
        uint8_t hue;
    public:
        virtual void clean() override;
        virtual void fill() override;
};

#endif