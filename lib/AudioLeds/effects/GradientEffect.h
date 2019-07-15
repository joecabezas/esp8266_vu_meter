#ifndef MULTIPLE_COLORS_EFFECT_H
#define MULTIPLE_COLORS_EFFECT_H

#include "BaseVUEffect.h"

#include <vector>

class GradientEffect : public BaseVUEffect
{
private:
    std::vector<CRGB> *colors;

protected:
    virtual void fill() override;
    CRGB getColor(uint8_t pixelIndex);

public:
    GradientEffect();
};

#endif