#ifdef CLIENT
// pinout documentation found at:
// https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/I2SInput/I2SInput.ino

#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "AudioLeds.h"
#include "effects/RainbowEffect.h"

#ifndef DEBUG
#undef DEBUG_NETWORK
#undef DEBUG_VOLUME
#endif

AudioLeds *audioLeds;
WiFiUDP *udp;

void setup()
{
#ifdef DEBUG
    USE_SERIAL.begin(9600);
#endif

#ifdef DEBUG_NETWORK
    USE_SERIAL.setDebugOutput(true);

    for (uint8_t t = 4; t > 0; t--)
    {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
#endif

    //The following line is fucking important and not documented anywhere
    //after a 3 year (and ongoing) issue, finally somebody came with this
    //solution
    //https://github.com/Links2004/arduinoWebSockets/issues/102
    WiFi.mode(WIFI_STA);

    WiFi.begin(MICROPHONE_STATION_SSID, MICROPHONE_STATION_PASSWORD);
#ifdef DEBUG_NETWORK
    USE_SERIAL.print("Connecting to ");
    USE_SERIAL.print(MICROPHONE_STATION_SSID);
    USE_SERIAL.println(" ...");
#endif

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
#ifdef DEBUG_NETWORK
        USE_SERIAL.print('>');
#endif
    }

#ifdef DEBUG_NETWORK
    USE_SERIAL.println('\n');
    USE_SERIAL.println("Connection established!");
    USE_SERIAL.print("Connected to ");
    USE_SERIAL.println(WiFi.SSID());
    USE_SERIAL.print("IP address:\t");
    USE_SERIAL.println(WiFi.localIP());
    USE_SERIAL.print("WiFi status:\t");
    USE_SERIAL.println(WiFi.status());
#endif

    if (WiFi.status() == WL_CONNECTED)
    {
        IPAddress broadcastAddress = IPAddress();
        broadcastAddress.fromString(UDP_MULTICAST_IP);

#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("UDP beginMulticast %s:%d\n", broadcastAddress.toString().c_str(), UDP_PORT);
#endif
        udp = new WiFiUDP();
        udp->beginMulticast(WiFi.localIP(), broadcastAddress, UDP_PORT);
    }

    audioLeds = new AudioLeds();

    RainbowEffect *rainbowEffect = new RainbowEffect();
    audioLeds->addEffect(rainbowEffect);
}

void loop()
{
    int packetSize = udp->parsePacket();
    if (packetSize)
    {
#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("Received %d bytes from %s, port %d\n", packetSize, udp->remoteIP().toString().c_str(), udp->remotePort());
#endif
        int volume = udp->read();
#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("Received value: %d\n", volume);
#endif
#ifdef DEBUG_VOLUME
        USE_SERIAL.println(volume);
#endif
        if (volume > 0)
        {
            audioLeds->loop(volume);
            return;
        }
    }

    audioLeds->loop(0);
}
#endif