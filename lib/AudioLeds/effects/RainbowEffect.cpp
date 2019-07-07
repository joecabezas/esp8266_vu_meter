#include "RainbowEffect.h"

#define FADE_BY 25
#define HUE_VELOCITY 4
#define DELTA_HUE 7

void RainbowEffect::loop(uint8_t fill_size)
{
    fadeToBlackBy(FastLED.leds(), FastLED.size(), FADE_BY);
    fill_rainbow(
        FastLED.leds(),
        _min(
            FastLED.size(),
            map(fill_size, 0, 255, -1, FastLED.size())),
        hue,
        DELTA_HUE);

    EVERY_N_MILLISECONDS(10) { hue += HUE_VELOCITY; }
}