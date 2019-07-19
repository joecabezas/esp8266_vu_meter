#include "BaseEffect.h"

#define INPUT_VELOCITY -5

void BaseEffect::setInputValue(uint8_t inputValue)
{
    input = _max(input, (float)inputValue);
}

uint8_t BaseEffect::getFillValue()
{
    return _min(
        FastLED.size(),
        map((uint8_t)input, 0, 255, 0, FastLED.size()));
}

void BaseEffect::clean()
{
    fill_solid(FastLED.leds(), FastLED.size(), CRGB::Black);
}

void BaseEffect::loop()
{
    clean();
    input = _max(0.0f, input + INPUT_VELOCITY);
    fill();
}
