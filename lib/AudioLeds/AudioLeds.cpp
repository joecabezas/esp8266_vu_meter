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

void AudioLeds::selectEffect(uint8_t index)
{
    if (effects.empty())
        return;

    currentEffectIndex = constrain(index, 0, NUMBER_OF_EFFECTS - 1);
    currentEffect = effects.at(currentEffectIndex);
}

void AudioLeds::previousEffect()
{
#ifdef DEBUG_AUDIO_LEDS
    Serial.println("previousEffect");
    Serial.println(currentEffectIndex);
#endif

    if (currentEffect)
        currentEffect->sleep();

    currentEffectIndex = (currentEffectIndex == 0) ? (effects.size() - 1) : currentEffectIndex - 1;
    currentEffect = effects.at(currentEffectIndex);

#ifdef DEBUG_AUDIO_LEDS
    Serial.println(currentEffectIndex);
#endif
}

void AudioLeds::nextEffect()
{
#ifdef DEBUG_AUDIO_LEDS
    Serial.println("nextEffect");
    Serial.println(currentEffectIndex);
#endif

    if (currentEffect)
        currentEffect->sleep();

    currentEffectIndex = (currentEffectIndex == (effects.size() - 1)) ? 0 : currentEffectIndex + 1;
    currentEffect = effects.at(currentEffectIndex);

#ifdef DEBUG_AUDIO_LEDS
    Serial.println(currentEffectIndex);
#endif
}

void AudioLeds::setInputValue(uint8_t fillValue)
{
    currentEffect->setInputValue(fillValue);
}

void AudioLeds::loop()
{
    currentEffect->loop();

    //this calls FastLED.show() as noted in docs
    //but I am noting this here if I ever search
    //for the show function call
    FastLED.delay(1);
}
