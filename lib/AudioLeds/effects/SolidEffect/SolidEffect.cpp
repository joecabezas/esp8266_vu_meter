#include "SolidEffect.h"

#define FADE_BY 20
#define SOLID_EFFECT_COLOR CRGB::Red

void SolidEffect::clean()
{
    fadeToBlackBy(FastLED.leds(), NUMBER_OF_LEDS, FADE_BY);
}

void SolidEffect::fill()
{

    fill_solid(
        FastLED.leds(),
        _min(
            NUMBER_OF_LEDS,
            map((uint8_t)input, 0, 255, -1, NUMBER_OF_LEDS)),
        SOLID_EFFECT_COLOR);
}