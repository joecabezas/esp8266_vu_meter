#ifdef BOTH
// pinout documentation found at:
// https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/I2SInput/I2SInput.ino

#include <Arduino.h>
#include <FastLED.h>

#include "AudioLeds.h"
#include "effects/RainbowEffect.h"

#include "Microphone.h"

#ifndef DEBUG
#undef DEBUG_VOLUME
#endif

Microphone *microphone;

void setup()
{
#ifdef DEBUG
    USE_SERIAL.begin(9600);
    USE_SERIAL.setDebugOutput(true);
#endif

    microphone = new Microphone(16000);
    audioLeds = new AudioLeds();

    RainbowEffect *rainbowEffect = new RainbowEffect();
    audioLeds->addEffect(rainbowEffect);
}

void loop()
{
    uint8_t volume = microphone->get8BitVolume();
    audioLeds->loop(volume);

    delay(2);
}
#endif