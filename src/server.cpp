#ifdef SERVER

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "Ticker.h"

#include "Microphone.h"

#ifndef DEBUG
#undef DEBUG_NETWORK
#endif

Microphone *microphone;
WiFiUDP *udp;
Ticker *volumeSenderTimer;

#ifdef UDP_MODE_BROADCAST
IPAddress *broadcastAddress;
#else
#include <vector>
std::vector<IPAddress *> *clients;
#endif

void sendVolume()
{
    uint8_t volume = microphone->getValue();
    if (!volume)
        return;

#ifdef DEBUG_NETWORK
    USE_SERIAL.printf("UDP about to send: %d...", volume);
#endif

#ifdef UDP_MODE_BROADCAST
    udp->beginPacketMulticast(*broadcastAddress, UDP_PORT, WiFi.softAPIP());
    udp->write(volume);
    udp->endPacket();
#else
    for (size_t i = 0; i < clients->size(); i++)
    {
        udp->beginPacket(*(clients->at(i)), UDP_PORT);
        udp->write(volume);
        udp->endPacket();
    }
#endif

#ifdef DEBUG_NETWORK
    USE_SERIAL.println("sent");
#endif
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

    volumeSenderTimer = new Ticker();
    volumeSenderTimer->attach_ms(30, sendVolume);
}

void loop()
{
    microphone->loop();
}
#endif