#ifdef SERVER

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "Microphone.h"

#ifndef DEBUG
#undef DEBUG_NETWORK
#endif

Microphone *microphone;
WiFiUDP *udp;

IPAddress *broadcastAddress;

void setup()
{
#ifdef DEBUG
    USE_SERIAL.begin(9600);
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
    WiFi.mode(WIFI_AP);
    WiFi.softAP(MICROPHONE_STATION_SSID, MICROPHONE_STATION_PASSWORD);
#ifdef DEBUG_NETWORK
    USE_SERIAL.print("Wifi softAPIP:\t");
    USE_SERIAL.println(WiFi.softAPIP());

    USE_SERIAL.print("Wifi localIP:\t");
    USE_SERIAL.println(WiFi.localIP());
#endif

    broadcastAddress = new IPAddress();
    broadcastAddress->fromString(UDP_MULTICAST_IP);

#ifdef DEBUG_NETWORK
    USE_SERIAL.printf("UDP broadcastAddress %s:%d\n", broadcastAddress->toString().c_str(), UDP_PORT);
#endif

    udp = new WiFiUDP();
    microphone = new Microphone(16000);
}

void loop()
{
    uint8_t volume = microphone->get8BitVolume();
    if (volume<30) return;

#ifdef DEBUG_NETWORK
    USE_SERIAL.printf("UDP about to send: %d...", volume);
#endif

    udp->beginPacketMulticast(*broadcastAddress, UDP_PORT, WiFi.softAPIP());
    udp->write(volume);
    udp->endPacket();

#ifdef DEBUG_NETWORK
    USE_SERIAL.println("sent");
#endif

    delay(2);
}
#endif