#ifdef BOTH

#include "Ticker.h"
#include "OneButton.h"

#include "AudioLeds.h"
#include "effects/RainbowEffect/RainbowEffect.h"
#include "effects/SolidEffect/SolidEffect.h"
#include "effects/RainbowVelocityEffect/RainbowVelocityEffect.h"
#include "effects/GradientEffect/GradientEffect.h"
#include "effects/FireEffect/FireEffect.h"
#include "effects/BouncingParticleEffect/BouncingParticleEffect.h"

#include "Microphone.h"

Microphone *microphone;
AudioLeds *audioLeds;
Ticker *volumeSenderTimer;
OneButton *button;

#ifdef TEST_EFFECTS
uint8_t theta;
#endif

void sendVolume()
{
#ifdef TEST_EFFECTS
    theta += 10;
    audioLeds->setInputValue(sin8(theta));
    return;
#endif

    uint8_t volume = microphone->getValue();
    if (!volume)
        return;

    audioLeds->setInputValue(volume);
}

void previousEffect()
{
    audioLeds->previousEffect();
}

void nextEffect()
{
    audioLeds->nextEffect();
}

void setup()
{
#ifdef DEBUG
    USE_SERIAL.begin(SERIAL_BAUD);
    USE_SERIAL.setDebugOutput(true);
#endif

    microphone = new Microphone();

    button = new OneButton(BUTTON_NEXT_EFFECT_PIN, true);
    button->attachDoubleClick(previousEffect);
    button->attachClick(nextEffect);

    audioLeds = new AudioLeds();
    audioLeds->addEffect(new RainbowEffect());
    audioLeds->addEffect(new SolidEffect());
    audioLeds->addEffect(new RainbowVelocityEffect());
    audioLeds->addEffect(new GradientEffect());
    audioLeds->addEffect(new FireEffect());
    audioLeds->addEffect(new BouncingParticleEffect());

    volumeSenderTimer = new Ticker();
    volumeSenderTimer->attach_ms(30, sendVolume);
}

void loop()
{
    button->tick();
    microphone->loop();
    audioLeds->loop();
}
#endif