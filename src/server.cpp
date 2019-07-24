#ifdef SERVER
// pinout documentation found at:
// https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/I2SInput/I2SInput.ino

// PINOUT used:
// SD  -> D6
// WS  -> D5
// SCK -> D7

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "NetworkPacket.h"

#include "Ticker.h"
#include "OneButton.h"

#include "Microphone.h"

#ifndef DEBUG
#undef DEBUG_NETWORK
#endif

Microphone *microphone;
WiFiUDP *udp;
Ticker *tickerSendVolume;
Ticker *tickersendSyncEffect;
OneButton *button;

uint8_t currentEffectIndex = 0;

#ifdef UDP_MODE_BROADCAST
IPAddress *broadcastAddress;
#else
#include <vector>
std::vector<IPAddress *> *clients;
#endif

#ifdef TEST_EFFECTS
#include <FastLED.h>
uint8_t theta;
#endif

void sendPacket(uint8_t command, uint8_t data)
{
    NetworkPacket packet;
    packet.command = command;
    packet.data = data;

#ifdef UDP_MODE_BROADCAST
    //this is outdated, because this is sending the volume (data)
    //only and since 20190720 I am sending a truct of 16bytes with
    //a command and the data, BUT, client is also outdated, so it
    //shall work, but now BROADCAST is not supported
    udp->beginPacketMulticast(*broadcastAddress, UDP_PORT, WiFi.softAPIP());
    udp->write(data);
    udp->endPacket();
#else

#ifdef DEBUG_NETWORK
    USE_SERIAL.printf("UDP sending: %d, %d\n", packet.command, packet.data);
#endif

    for (size_t i = 0; i < clients->size(); i++)
    {
        udp->beginPacket(*(clients->at(i)), UDP_PORT);
        udp->write((char *)&packet, sizeof(struct NetworkPacket));
        udp->endPacket();
    }
#endif
}

void sendPacket(uint8_t command)
{
    sendPacket(command, 0);
}

void sendVolume()
{
#ifdef TEST_EFFECTS
    theta += 10;
    sendPacket(COMMAND_MIC_DATA, sin8(theta));
    return;
#endif

    uint8_t volume = microphone->getValue();
    if (!volume)
        return;

#ifdef DEBUG_NETWORK
    USE_SERIAL.printf("UDP about to send: %d...", volume);
#endif

    sendPacket(COMMAND_MIC_DATA, volume);

#ifdef DEBUG_NETWORK
    USE_SERIAL.println("sent");
#endif
}

void sendSyncEffect()
{
    sendPacket(COMMAND_SYNC_EFFECT_INDEX, currentEffectIndex);
}

void previousEffect()
{
    currentEffectIndex = (currentEffectIndex == 0) ? (NUMBER_OF_EFFECTS - 1) : currentEffectIndex - 1;
    sendSyncEffect();
}

void nextEffect()
{
    currentEffectIndex = (currentEffectIndex == (NUMBER_OF_EFFECTS - 1)) ? 0 : currentEffectIndex + 1;
    sendSyncEffect();
}

void setup()
{
#ifdef DEBUG
    USE_SERIAL.begin(115200);
    USE_SERIAL.setDebugOutput(true);
#endif

#ifdef DEBUG_NETWORK
    for (uint8_t t = 4; t > 0; t--)
    {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
#endif

    WiFi.persistent(false);
    WiFi.disconnect(true);

    WiFi.mode(WIFI_AP);
    WiFi.softAP(MICROPHONE_STATION_SSID, MICROPHONE_STATION_PASSWORD);

#ifdef DEBUG_NETWORK
    USE_SERIAL.print("Wifi softAPIP:\t");
    USE_SERIAL.println(WiFi.softAPIP());

    USE_SERIAL.print("Wifi localIP:\t");
    USE_SERIAL.println(WiFi.localIP());
#endif

#ifdef UDP_MODE_BROADCAST
    broadcastAddress = new IPAddress();
    broadcastAddress->fromString(UDP_MULTICAST_IP);

#ifdef DEBUG_NETWORK
    USE_SERIAL.printf("UDP broadcastAddress %s:%d\n", broadcastAddress->toString().c_str(), UDP_PORT);
#endif
#else
    clients = new std::vector<IPAddress *>();
    for (size_t i = 0; i < NUMBER_OF_CLIENTS; i++)
    {
        clients->push_back(new IPAddress(192, 168, 4, 2 + i));
#ifdef DEBUG
        printf("added IP: %s\n", clients->back()->toString().c_str());
#endif
    }
#endif

    udp = new WiFiUDP();
    microphone = new Microphone();

    button = new OneButton(BUTTON_NEXT_EFFECT_PIN, true);
    button->attachDoubleClick(previousEffect);
    button->attachClick(nextEffect);

    tickerSendVolume = new Ticker();
    tickerSendVolume->attach_ms(30, sendVolume);

    tickersendSyncEffect = new Ticker();
    tickersendSyncEffect->attach_ms(1000, sendSyncEffect);
}

void loop()
{
    button->tick();
    microphone->loop();
}
#endif