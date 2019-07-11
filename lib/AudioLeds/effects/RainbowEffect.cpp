#include "RainbowEffect.h"

#define FADE_BY 15
#define DELTA_HUE 7

#define HUE_VELOCITY 1
#define FILL_VALUE_VELOCITY (-FastLED.size() * 0.22f)

void RainbowEffect::setFillValue(uint8_t a_fillValue)
{
    fillValue = _max(fillValue, (float)a_fillValue);
}

void RainbowEffect::loop()
{
    fadeToBlackBy(FastLED.leds(), FastLED.size(), FADE_BY);

    fill_rainbow(
        FastLED.leds(),
        _min(
            FastLED.size(),
            map((uint8_t)fillValue, 0, 255, -1, FastLED.size())),
        hue,
        DELTA_HUE);

    hue += HUE_VELOCITY;
    fillValue = _max(0.0f, fillValue + FILL_VALUE_VELOCITY);

#ifdef DEBUG_RAINBOW_EFFECT
    USE_SERIAL.print(fillValue);
    USE_SERIAL.print(",");
#endif
}