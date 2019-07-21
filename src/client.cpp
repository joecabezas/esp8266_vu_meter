#ifdef CLIENT
// pinout documentation found at:
// https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/I2SInput/I2SInput.ino

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "NetworkPacket.h"

#include "AudioLeds.h"
#include "effects/RainbowEffect/RainbowEffect.h"
#include "effects/SolidEffect/SolidEffect.h"
#include "effects/RainbowVelocityEffect/RainbowVelocityEffect.h"
#include "effects/GradientEffect/GradientEffect.h"
#include "effects/FireEffect/FireEffect.h"
#include "effects/BouncingParticleEffect/BouncingParticleEffect.h"

#ifndef DEBUG
#undef DEBUG_NETWORK
#undef DEBUG_MICROPHONE
#endif

AudioLeds *audioLeds;
WiFiUDP *udp;
NetworkPacket packet;

void readMicData(uint8_t volume)
{
    if (volume <= 0)
        return;

#ifdef DEBUG_MICROPHONE
    USE_SERIAL.println(volume);
#endif

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

void getPacket()
{
    int packetSize = udp->parsePacket();
    if (packetSize)
    {
#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("Received %d bytes from %s, port %d\n", packetSize, udp->remoteIP().toString().c_str(), udp->remotePort());
#endif

        udp->read((char *)&packet, sizeof(struct NetworkPacket));

#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("Received packet: %d, %d\n", packet.command, packet.data);
#endif

        if (packet.command == COMMAND_MIC_DATA)
            readMicData(packet.data);

        if (packet.command == COMMAND_PREVIOUS_EFFECT)
            previousEffect();

        if (packet.command == COMMAND_NEXT_EFFECT)
            nextEffect();
    }
}

void setup()
{
#ifdef DEBUG
    USE_SERIAL.begin(115200);
#endif

#ifdef DEBUG_NETWORK
    USE_SERIAL.setDebugOutput(true);

    for (uint8_t t = 1; t > 0; t--)
    {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
#endif

    WiFi.persistent(false);
    WiFi.disconnect(true);

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

    //todo: check if this if is necessary
    if (WiFi.status() == WL_CONNECTED)
    {
        udp = new WiFiUDP();
#ifdef UDP_MODE_BROADCAST
        IPAddress broadcastAddress = IPAddress();
        broadcastAddress.fromString(UDP_MULTICAST_IP);

        udp->beginMulticast(WiFi.localIP(), broadcastAddress, UDP_PORT);

#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("UDP beginMulticast %s:%d\n", broadcastAddress.toString().c_str(), UDP_PORT);
#endif
#else
        udp->begin(UDP_PORT);
#endif
    }

    audioLeds = new AudioLeds();
    audioLeds->addEffect(new RainbowEffect());
    audioLeds->addEffect(new BouncingParticleEffect());
    audioLeds->addEffect(new SolidEffect());
    audioLeds->addEffect(new GradientEffect());
    audioLeds->addEffect(new RainbowVelocityEffect());
    audioLeds->addEffect(new FireEffect());
}

void loop()
{
    audioLeds->loop();
    getPacket();
}
#endif