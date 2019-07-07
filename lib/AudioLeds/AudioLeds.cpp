#include "AudioLeds.h"

#define DATA_PIN 5
#define NUM_LEDS 18

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define BRIGHTNESS 96
#define FRAMES_PER_SECOND 120

#define DEBUG

AudioLeds::AudioLeds()
{
    currentEffectIndex = 0;

    leds = new CRGB[NUM_LEDS];
    FastLED
        .addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection(TypicalLEDStrip);

    FastLED.setBrightness(BRIGHTNESS);
}

void AudioLeds::addEffect(AbstractEffect *effect)
{
    effects.push_back(effect);
}

void AudioLeds::nextEffect()
{
#ifdef DEBUG
    Serial.println("nextEffect: " + currentEffectIndex);
#endif
    currentEffectIndex = (currentEffectIndex == effects.size()) ? 0 : currentEffectIndex + 1;
#ifdef DEBUG
    Serial.println("nextEffect: " + currentEffectIndex);
#endif
}

void AudioLeds::loop(uint8_t fill_size)
{
    FastLED.delay(1000 / FRAMES_PER_SECOND);
    getCurrentEffect()->loop(fill_size);
    FastLED.show();
}

AbstractEffect *AudioLeds::getCurrentEffect()
{
    return effects[currentEffectIndex];
}
