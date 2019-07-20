#include "RainbowEffect.h"

#define FADE_BY 20
#define DELTA_HUE 3
#define HUE_VELOCITY 1

void RainbowEffect::clean()
{
    fadeToBlackBy(FastLED.leds(), NUMBER_OF_LEDS, FADE_BY);
}

void RainbowEffect::fill()
{
    fill_rainbow(
        FastLED.leds(),
        _min(
            NUMBER_OF_LEDS,
            map((uint8_t)input, 0, 255, -1, NUMBER_OF_LEDS)),
        hue,
        DELTA_HUE);

    hue += HUE_VELOCITY;

#ifdef DEBUG_RAINBOW_EFFECT
    USE_SERIAL.println(input);
#endif
}