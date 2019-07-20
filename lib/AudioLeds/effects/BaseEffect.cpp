#include "BaseEffect.h"

#define INPUT_VELOCITY -5

void BaseEffect::setInputValue(uint8_t inputValue)
{
    input = _max(input, (float)inputValue);
}

uint8_t BaseEffect::getFillValue()
{
    return _min(
        NUMBER_OF_LEDS,
        map((uint8_t)input, 0, 255, 0, NUMBER_OF_LEDS));
}

void BaseEffect::clean()
{
    fill_solid(FastLED.leds(), NUMBER_OF_LEDS, CRGB::Black);
}

void BaseEffect::updateInput()
{
    input = _max(0.0f, input + INPUT_VELOCITY);
}

void BaseEffect::loop()
{
    clean();
    updateInput();
    fill();
}
