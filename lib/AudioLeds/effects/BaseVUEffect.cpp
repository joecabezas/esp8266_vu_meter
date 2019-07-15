#include "BaseVUEffect.h"

#define FADE_BY 20
#define INPUT_VELOCITY -5

void BaseVUEffect::setInputValue(uint8_t inputValue)
{
    input = _max(input, (float)inputValue);
}

uint8_t BaseVUEffect::getFillValue()
{
    return _min(
        FastLED.size(),
        map((uint8_t)input, 0, 255, 0, FastLED.size()));
}

void BaseVUEffect::loop()
{
    fadeToBlackBy(FastLED.leds(), FastLED.size(), FADE_BY);
    input = _max(0.0f, input + INPUT_VELOCITY);
    fill();
}
