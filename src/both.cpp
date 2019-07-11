#ifdef BOTH
// pinout documentation found at:
// https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/I2SInput/I2SInput.ino

#include <Arduino.h>
#include <FastLED.h>

#include "Ticker.h"

#include "AudioLeds.h"
#include "effects/RainbowEffect.h"

#include "Microphone.h"

Microphone *microphone;
AudioLeds *audioLeds;
Ticker *volumeSenderTimer;

void sendVolume()
{
    uint8_t volume = microphone->getValue();
    // volume = 255;
    if (!volume)
        return;

    audioLeds->setFillValue(volume);
}

void setup()
{
#ifdef DEBUG
    USE_SERIAL.begin(9600);
    USE_SERIAL.setDebugOutput(true);
#endif

    microphone = new Microphone();
    audioLeds = new AudioLeds();

    RainbowEffect *rainbowEffect = new RainbowEffect();
    audioLeds->addEffect(rainbowEffect);

    volumeSenderTimer = new Ticker();
    volumeSenderTimer->attach_ms(30, sendVolume);
}

void loop()
{
    microphone->loop();
    audioLeds->loop();
}
#endif