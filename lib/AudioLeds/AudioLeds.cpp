#include "AudioLeds.h"

#define DATA_PIN 5
#define NUM_LEDS 144

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

    if (effects.empty())
        currentEffect = effect;

    effects.push_back(effect);
}

void AudioLeds::nextEffect()
{
#ifdef DEBUG
    Serial.println("nextEffect: " + currentEffectIndex);
#endif
    currentEffectIndex = (currentEffectIndex == (effects.size() - 1)) ? 0 : currentEffectIndex + 1;
    currentEffect = effects[currentEffectIndex];
#ifdef DEBUG
    Serial.println("nextEffect: " + currentEffectIndex);
#endif
}

void AudioLeds::setFillValue(uint8_t fillValue)
{
    currentEffect->setFillValue(fillValue);
}

void AudioLeds::loop()
{
    currentEffect->loop();

    //this calls FastLED.show() as noted in docs
    //but I am noting this here if I ever search
    //for the show function call
    FastLED.delay(1);
}
