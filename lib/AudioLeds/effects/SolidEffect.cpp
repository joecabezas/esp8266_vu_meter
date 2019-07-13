#include "SolidEffect.h"

#define SOLID_EFFECT_COLOR CRGB::Red

#define FADE_BY 20
#define FILL_VALUE_VELOCITY -5

void SolidEffect::setFillValue(uint8_t a_fillValue)
{
    fillValue = _max(fillValue, (float)a_fillValue);
}

void SolidEffect::loop()
{
    fadeToBlackBy(FastLED.leds(), FastLED.size(), FADE_BY);
    fill_solid(FastLED.leds(),
               _min(
                   FastLED.size(),
                   map((uint8_t)fillValue, 0, 255, -1, FastLED.size())),
               SOLID_EFFECT_COLOR);
    fillValue = _max(0.0f, fillValue + FILL_VALUE_VELOCITY);
}