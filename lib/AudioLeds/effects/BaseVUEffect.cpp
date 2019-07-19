#include "BaseVUEffect.h"

#define FADE_BY 20

void BaseVUEffect::clean()
{
    fadeToBlackBy(FastLED.leds(), FastLED.size(), FADE_BY);
}
