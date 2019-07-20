#include "effects/RainbowVelocityEffect/RainbowVelocityEffect.h"

#define DELTA_HUE 3
#define VELOCITY_MAX 4.5f

#define VELOCITY map_generic((float)input, 0.0f, 255.0f, 0.0f, VELOCITY_MAX)

void RainbowVelocityEffect::clean()
{
}

void RainbowVelocityEffect::fill()
{
    fill_rainbow(
        FastLED.leds(),
        FastLED.size(),
        hue,
        DELTA_HUE);

    hue += VELOCITY;

#ifdef DEBUG_EFFECT
    USE_SERIAL.println(VELOCITY);
#endif
}