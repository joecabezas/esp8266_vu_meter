#include "AudioLeds.h"

#ifndef LEDS_DATA_PIN
#define LEDS_DATA_PIN 5
#endif

#ifndef NUMBER_OF_LEDS
#define NUMBER_OF_LEDS 100
#endif

#ifndef LED_TYPE
#define LED_TYPE WS2812B
#endif

#ifndef COLOR_ORDER
#define COLOR_ORDER GRB
#endif

#ifndef BRIGHTNESS
#define BRIGHTNESS 96
#endif

#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0

#define DEBUG

AudioLeds::AudioLeds()
{
    currentEffectIndex = 0;

    leds = new CRGB[NUMBER_OF_LEDS];
    FastLED
        .addLeds<LED_TYPE, LEDS_DATA_PIN, COLOR_ORDER>(leds, NUMBER_OF_LEDS)
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
