#ifdef BOTH
// pinout documentation found at:
// https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/I2SInput/I2SInput.ino

#include <FastLED.h>

#include "Ticker.h"

#include "AudioLeds.h"
#include "effects/RainbowEffect.h"
#include "effects/SolidEffect.h"
#include "effects/RainbowVelocityEffect.h"

#include "Microphone.h"

Microphone *microphone;
AudioLeds *audioLeds;
Ticker *volumeSenderTimer;

void sendVolume()
{
#ifdef TEST_EFFECTS
    audioLeds->setInputValue(255);
    return;
#endif

    uint8_t volume = microphone->getValue();
    if (!volume)
        return;

    audioLeds->setInputValue(volume);
}

void setup()
{
#ifdef DEBUG
    USE_SERIAL.begin(115200);
    USE_SERIAL.setDebugOutput(true);
#endif

    microphone = new Microphone();
    audioLeds = new AudioLeds();

    audioLeds->addEffect(new RainbowEffect());
    audioLeds->addEffect(new SolidEffect());
    audioLeds->addEffect(new RainbowVelocityEffect());

    audioLeds->nextEffect();
    audioLeds->nextEffect();

    volumeSenderTimer = new Ticker();
    volumeSenderTimer->attach_ms(30, sendVolume);
}

void loop()
{
    microphone->loop();
    audioLeds->loop();
}
#endif