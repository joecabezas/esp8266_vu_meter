#include "RainbowEffect.h"

#define FADE_BY 30
#define HUE_VELOCITY 5
#define DELTA_HUE 10

void RainbowEffect::loop(uint16_t fill_size)
{
    fadeToBlackBy(FastLED.leds(), FastLED.size(), FADE_BY);
    fill_rainbow(
        FastLED.leds(),
        _min(
            FastLED.size(),
            map(fill_size, 0, 450, -1, FastLED.size())),
        hue,
        DELTA_HUE);

    EVERY_N_MILLISECONDS(10) { hue += HUE_VELOCITY; }
}