#include "RainbowVelocityEffect.h"

#define DELTA_HUE 3
#define INPUT_VELOCITY -7.0f
#define VELOCITY_MAX 4.5f

#define VELOCITY map_generic((float)input, 0.0f, 255.0f, 0.0f, VELOCITY_MAX)

void RainbowVelocityEffect::setInputValue(uint8_t inputValue)
{
    input = _max(input, (float)inputValue);
}

void RainbowVelocityEffect::loop()
{
    fill_rainbow(
        FastLED.leds(),
        FastLED.size(),
        hue,
        DELTA_HUE);

    hue += VELOCITY;

    input = _max(0.0f, input + INPUT_VELOCITY);

#ifdef DEBUG_EFFECT
    USE_SERIAL.println(VELOCITY);
#endif
}