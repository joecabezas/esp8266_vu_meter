#ifndef GRADIENT_EFFECT_H
#define GRADIENT_EFFECT_H

#include "effects/BaseVUEffect.h"

#include <vector>

class GradientEffect : public BaseVUEffect
{
private:
    std::vector<CRGB> *colors;

protected:
    virtual void fill() override;

public:
    GradientEffect();
};

#endif