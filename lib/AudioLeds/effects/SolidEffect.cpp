#include "SolidEffect.h"

#define SOLID_EFFECT_COLOR CRGB::Red

#define FADE_BY 20
#define FILL_VALUE_VELOCITY -5

void SolidEffect::setInputValue(uint8_t inputValue)
{
    input = _max(inputValue, (float)inputValue);
}

void SolidEffect::loop()
{
    fadeToBlackBy(FastLED.leds(), FastLED.size(), FADE_BY);
    fill_solid(FastLED.leds(),
               _min(
                   FastLED.size(),
                   map((uint8_t)input, 0, 255, -1, FastLED.size())),
               SOLID_EFFECT_COLOR);
    input = _max(0.0f, input + FILL_VALUE_VELOCITY);
}