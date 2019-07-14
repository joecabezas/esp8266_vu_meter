#include "RainbowEffect.h"

#define FADE_BY 20
#define DELTA_HUE 3
#define HUE_VELOCITY 1
#define INPUT_VELOCITY -5

void RainbowEffect::setInputValue(uint8_t inputValue)
{
    input = _max(input, (float)inputValue);
}

void RainbowEffect::loop()
{
    fadeToBlackBy(FastLED.leds(), FastLED.size(), FADE_BY);

    fill_rainbow(
        FastLED.leds(),
        _min(
            FastLED.size(),
            map((uint8_t)input, 0, 255, -1, FastLED.size())),
        hue,
        DELTA_HUE);

    hue += HUE_VELOCITY;
    input = _max(0.0f, input + INPUT_VELOCITY);

#ifdef DEBUG_RAINBOW_EFFECT
    USE_SERIAL.print(input);
    USE_SERIAL.print(",");
#endif
}